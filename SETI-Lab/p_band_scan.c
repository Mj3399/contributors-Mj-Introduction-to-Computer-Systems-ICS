#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <sched.h>    // for processor affinity
#include <unistd.h>   // unix standard apis
#include <pthread.h>  // pthread api
#include <math.h>
#include "filter.h"
#include "signal.h"
#include "timing.h"

#define MAXWIDTH 40
#define THRESHOLD 2.0
#define ALIENS_LOW  50000.0
#define ALIENS_HIGH 150000.0

typedef struct TDATA {
  int band;
  double bandwidth;
  int bb;
  int last_thread;
  int filter_order;
  double* filter_coeffs;
  signal* sig;
  double* band_power;
} TDATA;
int num_threads;            // number of threads we will use
int num_procs;              // number of processors we will use
pthread_t* tid;             // array of thread ids
double *band_power;
double Fc;
double bandwidth;
void usage() {
  printf("usage: p_band_scan text|bin|mmap signal_file Fs filter_order num_bands num_threads num_processors\n");
}

double avg_power(double* data, int num) {

  double ss = 0;
  for (int i = 0; i < num; i++) {
    ss += data[i] * data[i];
  }

  return ss / num;
}

double max_of(double* data, int num) {

  double m = data[0];
  for (int i = 1; i < num; i++) {
    if (data[i] > m) {
      m = data[i];
    }
  }
  return m;
}

double avg_of(double* data, int num) {

  double s = 0;
  for (int i = 0; i < num; i++) {
    s += data[i];
  }
  return s / num;
}

void remove_dc(double* data, int num) {

  double dc = avg_of(data,num);

  printf("Removing DC component of %lf\n",dc);

  for (int i = 0; i < num; i++) {
    data[i] -= dc;
  }
}

void* worker(void* arg) {
  TDATA *tdata = (TDATA*) arg;

   

  int i;
  for (i=(tdata->band)*(tdata->bb); i<((tdata->band)*(tdata->bb))+(tdata->last_thread); i++) {
    // Make the filter
    int c = i * (tdata->filter_order + 1);
    generate_band_pass(tdata->sig->Fs,
                       (i * tdata->bandwidth) + 0.0001, // keep within limits
                       ((i+1) * tdata->bandwidth) - 0.0001,
                       tdata->filter_order,
                       &(tdata->filter_coeffs[c]));
    hamming_window(tdata->filter_order, &(tdata->filter_coeffs[(c)]));

    // Convolve
    convolve_and_compute_power(tdata->sig->num_samples,
                               tdata->sig->data,
                               tdata->filter_order,
                               &(tdata->filter_coeffs[(c)]),
                               &(tdata->band_power[i]));
    

  }

  pthread_exit(NULL); // finish - no return value
}

int analyze_signal(signal* sig, int filter_order, int num_bands, double* lb, double* ub, int num_threads, int num_procs) {
  double Fc        = (sig->Fs) / 2;
  double bandwidth = Fc / num_bands;

  remove_dc(sig->data,sig->num_samples);

  double signal_power = avg_power(sig->data,sig->num_samples);

  printf("signal average power:     %lf\n", signal_power);

  resources rstart;
  get_resources(&rstart,THIS_PROCESS);
  double start = get_seconds();
  unsigned long long tstart = get_cycle_count();

  double filter_coeffs[(filter_order+1)*(num_bands)];
  double band_power[num_bands];

  tid = (pthread_t*)malloc(sizeof(pthread_t) * num_threads);
  if (!tid) {
    fprintf(stderr, "no memory\n");
    exit(-1);
  }
    // when less threads than bands, split
  int at;
  int bb;
  if (num_threads < num_bands) {
    at = num_threads;
    bb = floor(num_bands/num_threads);
  } else {
    at = num_bands;
    bb = 1;
  }

  int last_thread = bb;
  for (int band = 0; band < at; band++) {
    if (band == (at -1)){
      last_thread = (num_bands - (bb*band));
    }
    //last thread gets the uneven amount of threads if not split evenly!!!!
    TDATA* tdata = malloc(sizeof(TDATA));

    tdata->band = band;
    tdata->bandwidth = bandwidth;
    tdata->bb = bb;
    tdata->last_thread = last_thread;
    tdata->filter_order = filter_order;
    tdata->filter_coeffs = filter_coeffs;
    tdata->sig = sig;
    tdata->band_power = band_power;    
    int returncode = pthread_create(&(tid[band]),  // thread id gets put here
                                    NULL, // use default attributes
                                    worker, // thread will begin in this function
                                    tdata // pointer
                                    );
    if (returncode != 0) {
      perror("Failed to start thread");
      exit(-1);
    }
  }
  // now we will join all the threads
  for (int i = 0; i < at; i++) {
    int returncode = pthread_join(tid[i], NULL);
    if (returncode != 0) {
      perror("join failed");
      exit(-1);
    }
  }
  unsigned long long tend = get_cycle_count();
  double end = get_seconds();

  resources rend;
  get_resources(&rend,THIS_PROCESS);

  resources rdiff;
  get_resources_diff(&rstart, &rend, &rdiff);

  // Pretty print results
  double max_band_power = max_of(band_power,num_bands);
  double avg_band_power = avg_of(band_power,num_bands);
  int wow = 0;
  *lb = -1;
  *ub = -1;

  for (int band = 0; band < num_bands; band++) {
    double band_low  = band * bandwidth + 0.0001;
    double band_high = (band + 1) * bandwidth - 0.0001;

    printf("%5d %20lf to %20lf Hz: %20lf ",
           band, band_low, band_high, band_power[band]);

    for (int i = 0; i < MAXWIDTH * (band_power[band] / max_band_power); i++) {
      printf("*");
    }

    if ((band_low >= ALIENS_LOW && band_low <= ALIENS_HIGH) ||
        (band_high >= ALIENS_LOW && band_high <= ALIENS_HIGH)) {

      // band of interest
      if (band_power[band] > THRESHOLD * avg_band_power) {
        printf("(WOW)");
        wow = 1;
        if (*lb < 0) {
          *lb = band * bandwidth + 0.0001;
        }
        *ub = (band + 1) * bandwidth - 0.0001;
      } else {
        printf("(meh)");
      }
    } else {
      printf("(meh)");
    }

    printf("\n");
  }

  printf("Resource usages:\n"
         "User time        %lf seconds\n"
         "System time      %lf seconds\n"
         "Page faults      %ld\n"
         "Page swaps       %ld\n"
         "Blocks of I/O    %ld\n"
         "Signals caught   %ld\n"
         "Context switches %ld\n",
         rdiff.usertime,
         rdiff.systime,
         rdiff.pagefaults,
         rdiff.pageswaps,
         rdiff.ioblocks,
         rdiff.sigs,
         rdiff.contextswitches);

  printf("Analysis took %llu cycles (%lf seconds) by cycle count, timing overhead=%llu cycles\n"
         "Note that cycle count only makes sense if the thread stayed on one core\n",
         tend - tstart, cycles_to_seconds(tend - tstart), timing_overhead());
  printf("Analysis took %lf seconds by basic timing\n", end - start);

  return wow;
}

int main(int argc, char* argv[]) {

  if (argc != 8) {
    usage();
    return -1;
  }

  char sig_type    = toupper(argv[1][0]);
  char* sig_file   = argv[2];
  double Fs        = atof(argv[3]);
  int filter_order = atoi(argv[4]);
  int num_bands    = atoi(argv[5]);
  int num_threads = atoi(argv[6]);      // number of threads
  int num_procs   = atoi(argv[7]);    // numer of processors to use

  assert(Fs > 0.0);
  assert(filter_order > 0 && !(filter_order & 0x1));
  assert(num_bands > 0);

  printf("type:     %s\n"
         "file:     %s\n"
         "Fs:       %lf Hz\n"
         "order:    %d\n"
         "bands:    %d\n",
         sig_type == 'T' ? "Text" : (sig_type == 'B' ? "Binary" : (sig_type == 'M' ? "Mapped Binary" : "UNKNOWN TYPE")),
         sig_file,
         Fs,
         filter_order,
         num_bands);

  printf("Load or map file\n");

  signal* sig;
  switch (sig_type) {
    case 'T':
      sig = load_text_format_signal(sig_file);
      break;

    case 'B':
      sig = load_binary_format_signal(sig_file);
      break;

    case 'M':
      sig = map_binary_format_signal(sig_file);
      break;

    default:
      printf("Unknown signal type\n");
      return -1;
  }

  if (!sig) {
    printf("Unable to load or map file\n");
    return -1;
  }

  sig->Fs = Fs;

  double start = 0;
  double end   = 0;
  if (analyze_signal(sig, filter_order, num_bands, &start, &end, num_threads, num_procs)) {
    printf("POSSIBLE ALIENS %lf-%lf HZ (CENTER %lf HZ)\n", start, end, (end + start) / 2.0);
  } else {
    printf("no aliens\n");
  }

  free_signal(sig);

  return 0;
}


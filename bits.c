 /* 
 * CS:APP Data Lab 
 * * bits.c - Source file with your solutions to the Lab.
 * This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 */
#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */
You will provide your solution to the Data Lab by
editing the collection of functions in this source file.
INTEGER CODING RULES:
 Replace the "return" statement in each function with one
 or more lines of C code that implements the function. Your code
 must conform to the following style:
 int Funct(arg1, arg2, ...) {
 /* brief description of how your implementation works */
 int var1 = Expr1;
 ...
 int varM = ExprM;
 varJ = ExprJ;
 ...
 varN = ExprN;
 return ExprR;
 }
 Each "Expr" is an expression using ONLY the following:
 1. Integer constants 0 through 255 (0xFF), inclusive. You are
 not allowed to use big constants such as 0xffffffff.
 2. Function arguments and local variables (no global variables).
 3. Unary integer operations ! ~
 4. Binary integer operations & ^ | + << >>

 Some of the problems restrict the set of allowed operators even further.
 Each "Expr" may consist of multiple operators. You are not restricted to
 one operator per line.
 You are expressly forbidden to:
 1. Use any control constructs such as if, do, while, for, switch, etc.
 2. Define or use any macros.
 3. Define any additional functions in this file.
 4. Call any functions.
 5. Use any other operations, such as &&, ||, -, or ?:
 6. Use any form of casting.
 7. Use any data type other than int. This implies that you
 cannot use arrays, structs, or unions.
 You may assume that your machine:
 1. Uses 2s complement, 32-bit representations of integers.
 2. Performs right shifts arithmetically.
 3. Has unpredictable behavior when shifting an integer by more
 than the word size.
EXAMPLES OF ACCEPTABLE CODING STYLE:
 /*
 * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
 */
 int pow2plus1(int x) {
 /* exploit ability of shifts to compute powers of 2 */
 return (1 << x) + 1;
 }
 /*
 * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
 */
 int pow2plus4(int x) {
 /* exploit ability of shifts to compute powers of 2 */
 int result = (1 << x);
 result += 4;
 return result;
 }
FLOATING POINT CODING RULES
For the problems that require you to implent floating-point operations,
the coding rules are less strict. You are allowed to use looping and
conditional control. You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.
You are expressly forbidden to:
 1. Define or use any macros.
 2. Define any additional functions in this file.
 3. Call any functions.
 4. Use any form of casting.
 5. Use any data type other than int or unsigned. This means that you
 cannot use arrays, structs, or unions.
 6. Use any floating point data types, operations, or constants.
NOTES:
 1. Use the dlc (data lab checker) compiler (described in the handout) to
 check the legality of your solutions.
 2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
 that you are allowed to use for your implementation of the function.
 The max operator count is checked by dlc. Note that '=' is not
 counted; you may use as many of these as you want without penalty.
 3. Use the btest test harness to check your functions for correctness.
 4. Use the BDD checker to formally verify your functions
 5. The maximum number of ops for each function is given in the
 header comment for each function. If there are any inconsistencies
 between the maximum ops in the writeup and in this file, consider
 this file the authoritative source.
/*
 * STEP 2: Modify the following functions according the coding rules.
 *
 * IMPORTANT. TO AVOID GRADING SURPRISES:
 * 1. Use the dlc compiler to check that your solutions conform
 * to the coding rules.
 * 2. Use the BDD checker to formally verify that your solutions produce
 * the correct answers.
 */
#endif
/* Copyright (C) 1991-2018 Free Software Foundation, Inc.
 This file is part of the GNU C Library.
 The GNU C Library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 The GNU C Library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.
 You should have received a copy of the GNU Lesser General Public
 License along with the GNU C Library; if not, see
 <http://www.gnu.org/licenses/>. */
/* This header is separate from features.h so that the compiler can
 include it implicitly at the start of every compilation. It must
 not itself include <features.h> or any other header that includes
 <features.h> because the implicit include comes before any feature
 test macros that may be defined in a source file before it first
 explicitly includes a system header. GCC knows the name of this
 header in order to preinclude it. */
/* glibc's intent is to support the IEC 559 math functionality, real
 and complex. If the GCC (4.9 and later) predefined macros
 specifying compiler intent are available, use them to determine
 whether the overall intent is to support these features; otherwise,
 presume an older compiler has intent to support these features and
 define these macros by default. */
/* wchar_t uses Unicode 10.0.0. Version 10.0 of the Unicode Standard is
 synchronized with ISO/IEC 10646:2017, fifth edition, plus
 the following additions from Amendment 1 to the fifth edition:
 - 56 emoji characters
 - 285 hentaigana
 - 3 additional Zanabazar Square characters */
/* Bit manipulations */
/*
 * bitAnd - x&y using only ~ and |
 * Example: bitAnd(6, 5) = 4
 * Legal ops: ~ |
 * Max ops: 8
 * Rating: 1
 */
int bitAnd(int x, int y) {
/* using ~ on x and y and then performing | on them and taking the compliment again
*/
 int b = ~y;
 int a = ~x;
 int result = a | b;
 result = ~result;
 return result;
}
/*
 * thirdBits - return word with every third bit (starting from the LSB) set to 1
 * Legal ops: ! ~ & ^ | + << >>
 * Max ops: 8
 * Rating: 1
 */
int thirdBits(void) {
/*returning a word with every third bit from the LSB set to 1 */
 int x = 0x49;

 x = x | (x << 9);

 x = x | (x << 18);

 return x;
}
/*
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 * Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 * Legal ops: ! ~ & ^ | + << >>
 * Max ops: 12
 * Rating: 2
 */
int allEvenBits(int x) {
 int even = 0x55;
 even = even | (even << 8);
 even = even | (even << 16);

 return !(x ^ (even | x));


}
/*
 * byteSwap - swaps the nth byte and the mth byte
 * Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 * byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 * You may assume that 0 <= n <= 3, 0 <= m <= 3
 * Legal ops: ! ~ & ^ | + << >>
 * Max ops: 25
 * Rating: 2
 */
int byteSwap(int x, int n, int m) {
 int X = x;
 int A = 0xFF;
 int N = n+n;
 int N1 = N+N;
 int N2 = N1 + N1;
 int M = m+m+m+m+m+m+m+m;
 int A1 = A << N2; /* going into n place */
 int A2 = A << M; /* going into m place */
 int A3 = A1 | A2; /* all zeros except for whats switching */
 int A4 = ~A3; /* all zeros except for whats staying the same */
 int Y = X >> M; /* taking mth byte */
 int Z = X >> N2; /* taking nth byte */
 int result = 0;
 Y = Y & A;
 Z = Z & A;
 Y = Y << N2; /* swtiching */
 Z = Z << M;
 X = X & A4;
 result = X | Y | Z;
 return result;
}
/*
 * conditional - same as x ? y : z
 * Example: conditional(2,4,5) = 4
 * Legal ops: ! ~ & ^ | + << >>
 * Max ops: 16
 * Rating: 3
 */
int conditional(int x, int y, int z) {
 /* 2 = 0b0010 = 0x02, 4 = 0b0100 = 0x04, 5 = 0b0101 = 0x05 */

 int tf = !!x; /* tells whether true or flase if x is o or 1 */
 return ((((tf << 31) >> 31) & y) | (((~tf << 31) >> 31)&z));
}
/*
 * bitParity - returns 1 if x contains an odd number of 0's
 * Examples: bitParity(5) = 0, bitParity(7) = 1
 * Legal ops: ! ~ & ^ | + << >>
 * Max ops: 20
 * Rating: 4
 */
int bitParity(int x) {
 x = ( x >> 16 ) ^ x;
 x = ( x >> 8 ) ^ x;
 x = ( x >> 4 ) ^ x;
 x = ( x >> 2 ) ^ x;
 x = ( x >> 1 ) ^ x;
 x = x & 1;
 return x;
}
/* Two's complement arithmetic */
/*
 * TMax - return maximum two's complement integer
 * Legal ops: ! ~ & ^ | + << >>
 * Max ops: 4
 * Rating: 1
 */
int tmax(void) {
 /* 0111 1111 1111 1111 1111 1111 1111 1111 */
 int one = 0x80;
 one = (one << 24);
 one = ~one;


 return one;
}
/*
 * sign - return 1 if positive, 0 if zero, and -1 if negative
 * Examples: sign(130) = 1
 * sign(-23) = -1
 * Legal ops: ! ~ & ^ | + << >>
 * Max ops: 10
 * Rating: 2
 */
int sign(int x) {

 return ((x>>31)|(!!x));
}
/*
 * isLessOrEqual - if x <= y then return 1, else return 0
 * Example: isLessOrEqual(4,5) = 1.
 * Legal ops: ! ~ & ^ | + << >>
 * Max ops: 24
 * Rating: 3
 */
int isLessOrEqual(int x, int y) {
 /* 0100 < 0101 */
 int result = 0;
 /* true or false on whether signs are the same */
 int tf = !(x>>31)^!(y>>31);
 /* different signs case if x is neg will then its true y is greater */
 int a = tf & (x>>31);
 /* same signs and y is greater */
 int b = !tf & !((y + (~x+1))>>31);
 result = a | b;
 return result;
}
/*
 * isPositive - return 1 if x > 0, return 0 otherwise
 * Example: isPositive(-1) = 0.
 * Legal ops: ! ~ & ^ | + << >>
 * Max ops: 8
 * Rating: 3
 */
int isPositive(int x) {
 return (~(x>>31)&(!!x));
}
/*
 * absVal - absolute value of x
 * Example: absVal(-1) = 1.
 * You may assume -TMax <= x <= TMax
 * Legal ops: ! ~ & ^ | + << >>
 * Max ops: 10
 * Rating: 4
 */
int absVal(int x) {
 int sign = x >> 31;
 x = x ^ sign;
 x = x + (~sign + 1);
 return x;
}
/* FP operations */
/*
 * float_neg - Return bit-level equivalent of expression -f for
 * floating point argument f.
 * Both the argument and result are passed as unsigned int's, but
 * they are to be interpreted as the bit-level representations of
 * single-precision floating point values.
 * When argument is NaN, return argument.
 * Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 * Max ops: 10
 * Rating: 2
 */
unsigned float_neg(unsigned uf) {
 unsigned J = 0x80;
 unsigned J1 = J << 24;
 int x;
 int exp = ((uf >> 23) & 0xff);
/* special value scenario */

 if (exp == 0xFF && ( (uf << 9) != 0x00 )){
 return uf;
 }
/* normalized value scenario */
 else{
 x = uf+J1;
 return x;
 }


}
/*
 * float_abs - Return bit-level equivalent of absolute value of f for
 * floating point argument f.
 * Both the argument and result are passed as unsigned int's, but
 * they are to be interpreted as the bit-level representations of
 * single-precision floating point values.
 * When argument is NaN, return argument..
 * Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 * Max ops: 10
 * Rating: 2
 */
unsigned float_abs(unsigned uf) {
 unsigned J = 0x80;
 unsigned J1 = J << 24;
 int x;
 int exp = ((uf >> 23) & 0xff);
 int s = uf >> 31;

/* special value scenario */

 if (exp == 0xFF && ( (uf << 9) != 0x00 )){
 return uf;
 }
/* normalized value scenario */
 else if ((s == 1)){
 x = uf+J1;
 return x;
 }

 return uf;
}
/*
 * float_f2i - Return bit-level equivalent of expression (int) f
 * for floating point argument f.
 * Argument is passed as unsigned int, but
 * it is to be interpreted as the bit-level representation of a
 * single-precision floating point value.
 * Anything out of range (including NaN and infinity) should return
 * 0x80000000u.
 * Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 * Max ops: 30
 * Rating: 4
 */
int float_f2i(unsigned uf) {
/*Isolate s, M, and exp */
/* shift important values to least significant bits and create masks */
 int s = uf >> 31;
 int exp = uf >> 23;
 int exp1 = 0xFF;
 int frac = 0;
 unsigned J = 0x80;
 unsigned J1 = J << 24;
 int x;
 int E;
/*create mask for M by shifting left while keeping 1s */
 int M1 = (exp1 << 8) | exp1;
 M1 = (M1 << 7) | M1;
 /* isolate */
 frac = uf & M1;
 exp = exp & exp1;
/* V = (-1)^s * M * 2^E */
/* denormalized value scenario */
 if (exp == 0x00){
 return 0;
 }
/* special value scenario */
 else if (exp == 0xFF){
 return J1;
 }
/* normalized value scenario */
/* don't forget implied leading 1 */
 else{
 x = 0;
 E = exp - 127;
/* frac exp round to zero */
 if (E<0){
 return 0;
 }
 /* overflow */
 if (E>30){
 return J1;
 }
 x = 1 << E;
 if (E < 23){
 x |= frac >> (23 - E);
 }else{
 x |= frac << (E - 23 );
 }
/* Negative values */
 if (s == 1){
 x = ~x + 1;
 }
 return x;
 }
}

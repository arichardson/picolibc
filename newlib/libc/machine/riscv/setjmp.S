/* Copyright (c) 2017  SiFive Inc. All rights reserved.

   This copyrighted material is made available to anyone wishing to use,
   modify, copy, or redistribute it subject to the terms and conditions
   of the FreeBSD License.   This program is distributed in the hope that
   it will be useful, but WITHOUT ANY WARRANTY expressed or implied,
   including the implied warranties of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  A copy of this license is available at
   http://www.opensource.org/licenses.
*/

#include <sys/asm.h>

/* int setjmp (jmp_buf);  */
  .section .text.setjmp
  .globl  setjmp
  .type   setjmp, @function
setjmp:
	REG_S GPR(ra),  0*SZREG(PTRREG(a0))
	REG_S GPR(s0),  1*SZREG(PTRREG(a0))
	REG_S GPR(s1),  2*SZREG(PTRREG(a0))

#ifndef __riscv_32e
	REG_S GPR(s2),  3*SZREG(PTRREG(a0))
	REG_S GPR(s3),  4*SZREG(PTRREG(a0))
	REG_S GPR(s4),  5*SZREG(PTRREG(a0))
	REG_S GPR(s5),  6*SZREG(PTRREG(a0))
	REG_S GPR(s6),  7*SZREG(PTRREG(a0))
	REG_S GPR(s7),  8*SZREG(PTRREG(a0))
	REG_S GPR(s8),  9*SZREG(PTRREG(a0))
	REG_S GPR(s9), 10*SZREG(PTRREG(a0))
	REG_S GPR(s10),11*SZREG(PTRREG(a0))
	REG_S GPR(s11),12*SZREG(PTRREG(a0))
	REG_S GPR(sp), 13*SZREG(PTRREG(a0))
#else
	REG_S GPR(sp), 3*SZREG(PTRREG(a0))
#endif

#ifndef __riscv_float_abi_soft
	FREG_S fs0, 14*SZREG+ 0*SZFREG(PTRREG(a0))
	FREG_S fs1, 14*SZREG+ 1*SZFREG(PTRREG(a0))
	FREG_S fs2, 14*SZREG+ 2*SZFREG(PTRREG(a0))
	FREG_S fs3, 14*SZREG+ 3*SZFREG(PTRREG(a0))
	FREG_S fs4, 14*SZREG+ 4*SZFREG(PTRREG(a0))
	FREG_S fs5, 14*SZREG+ 5*SZFREG(PTRREG(a0))
	FREG_S fs6, 14*SZREG+ 6*SZFREG(PTRREG(a0))
	FREG_S fs7, 14*SZREG+ 7*SZFREG(PTRREG(a0))
	FREG_S fs8, 14*SZREG+ 8*SZFREG(PTRREG(a0))
	FREG_S fs9, 14*SZREG+ 9*SZFREG(PTRREG(a0))
	FREG_S fs10,14*SZREG+10*SZFREG(PTRREG(a0))
	FREG_S fs11,14*SZREG+11*SZFREG(PTRREG(a0))
#endif

	li    a0, 0
	RET
	.size	setjmp, .-setjmp

/* volatile void longjmp (jmp_buf, int);  */
  .section .text.longjmp
  .globl  longjmp
  .type   longjmp, @function
longjmp:
	REG_L GPR(ra),  0*SZREG(PTRREG(a0))
	REG_L GPR(s0),  1*SZREG(PTRREG(a0))
	REG_L GPR(s1),  2*SZREG(PTRREG(a0))
#ifndef __riscv_32e
	REG_L GPR(s2),  3*SZREG(PTRREG(a0))
	REG_L GPR(s3),  4*SZREG(PTRREG(a0))
	REG_L GPR(s4),  5*SZREG(PTRREG(a0))
	REG_L GPR(s5),  6*SZREG(PTRREG(a0))
	REG_L GPR(s6),  7*SZREG(PTRREG(a0))
	REG_L GPR(s7),  8*SZREG(PTRREG(a0))
	REG_L GPR(s8),  9*SZREG(PTRREG(a0))
	REG_L GPR(s9), 10*SZREG(PTRREG(a0))
	REG_L GPR(s10),11*SZREG(PTRREG(a0))
	REG_L GPR(s11),12*SZREG(PTRREG(a0))
	REG_L GPR(sp), 13*SZREG(PTRREG(a0))
#else
	REG_L GPR(sp), 3*SZREG(PTRREG(a0))
#endif

#ifndef __riscv_float_abi_soft
	FREG_L fs0, 14*SZREG+ 0*SZFREG(PTRREG(a0))
	FREG_L fs1, 14*SZREG+ 1*SZFREG(PTRREG(a0))
	FREG_L fs2, 14*SZREG+ 2*SZFREG(PTRREG(a0))
	FREG_L fs3, 14*SZREG+ 3*SZFREG(PTRREG(a0))
	FREG_L fs4, 14*SZREG+ 4*SZFREG(PTRREG(a0))
	FREG_L fs5, 14*SZREG+ 5*SZFREG(PTRREG(a0))
	FREG_L fs6, 14*SZREG+ 6*SZFREG(PTRREG(a0))
	FREG_L fs7, 14*SZREG+ 7*SZFREG(PTRREG(a0))
	FREG_L fs8, 14*SZREG+ 8*SZFREG(PTRREG(a0))
	FREG_L fs9, 14*SZREG+ 9*SZFREG(PTRREG(a0))
	FREG_L fs10,14*SZREG+10*SZFREG(PTRREG(a0))
	FREG_L fs11,14*SZREG+11*SZFREG(PTRREG(a0))
#endif

	seqz a0, a1
	add  a0, a0, a1   # a0 = (a1 == 0) ? 1 : a1
	RET
	.size	longjmp, .-longjmp

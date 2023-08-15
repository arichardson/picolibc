/* Copyright (c) 2017  SiFive Inc. All rights reserved.

   This copyrighted material is made available to anyone wishing to use,
   modify, copy, or redistribute it subject to the terms and conditions
   of the FreeBSD License.   This program is distributed in the hope that
   it will be useful, but WITHOUT ANY WARRANTY expressed or implied,
   including the implied warranties of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  A copy of this license is available at
   http://www.opensource.org/licenses.
*/

#ifndef _SYS_ASM_H
#define _SYS_ASM_H

#ifndef __CONCAT
#define	__CONCAT1(x,y)	x ## y
#define	__CONCAT(x,y)	__CONCAT1(x,y)
#endif

/*
 * Macros to handle different pointer/register sizes for 32/64-bit code
 */
#ifdef __CHERI_PURE_CAPABILITY__
# define PTRLOG (2 + (__riscv_xlen / 32))
# define SZREG	(__riscv_xlen / 4)
# define REG_S csc
# define REG_L clc
#elif __riscv_xlen == 64
# define PTRLOG 3
# define PTRREG(n) n
# define SZREG	8
# define REG_S sd
# define REG_L ld
#elif __riscv_xlen == 32
# define PTRLOG 2
# define SZREG	4
# define REG_S sw
# define REG_L lw
#else
# error __riscv_xlen must equal 32 or 64
#endif

#ifdef __CHERI_PURE_CAPABILITY__
# define PTRREG(n) __CONCAT(c, n)
# define INTREG(n) n
# define GPR(n) PTRREG(n)
#else
# define PTRREG(n) n
# define INTREG(n) n
# define GPR(n) PTRREG(n)
#endif

#ifndef __riscv_float_abi_soft
#ifdef __CHERI_PURE_CAPABILITY__
#define FLDST_PREFIX c
#else
#define FLDST_PREFIX
#endif
/* For ABI uniformity, reserve 8 bytes for floats, even if double-precision
   floating-point is not supported in hardware.  */
# define SZFREG 8
# ifdef __riscv_float_abi_single
#  define FREG_L __CONCAT(FLDST_PREFIX, flw)
#  define FREG_S __CONCAT(FLDST_PREFIX, fsw)
# elif defined(__riscv_float_abi_double)
#  define FREG_L __CONCAT(FLDST_PREFIX, fld)
#  define FREG_S __CONCAT(FLDST_PREFIX, fsd)
# elif defined(__riscv_float_abi_quad)
#  define FREG_L __CONCAT(FLDST_PREFIX, flq)
#  define FREG_S __CONCAT(FLDST_PREFIX, fsq)
# else
#  error unsupported FLEN
# endif
#endif

#ifdef __CHERI_PURE_CAPABILITY__
#define RET cret
#else
#define RET ret
#endif

#endif /* sys/asm.h */

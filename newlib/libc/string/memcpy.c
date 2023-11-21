/*
Copyright (c) 1994 Cygnus Support.
All rights reserved.

Redistribution and use in source and binary forms are permitted
provided that the above copyright notice and this paragraph are
duplicated in all such forms and that any documentation,
and/or other materials related to such
distribution and use acknowledge that the software was developed
at Cygnus Support, Inc.  Cygnus Support, Inc. may not be used to
endorse or promote products derived from this software without
specific prior written permission.
THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/*
FUNCTION
        <<memcpy>>---copy memory regions

SYNOPSIS
        #include <string.h>
        void* memcpy(void *restrict <[out]>, const void *restrict <[in]>,
                     size_t <[n]>);

DESCRIPTION
        This function copies <[n]> bytes from the memory region
        pointed to by <[in]> to the memory region pointed to by
        <[out]>.

        If the regions overlap, the behavior is undefined.

RETURNS
        <<memcpy>> returns a pointer to the first byte of the <[out]>
        region.

PORTABILITY
<<memcpy>> is ANSI C.

<<memcpy>> requires no supporting OS subroutines.

QUICKREF
        memcpy ansi pure
	*/

#include <_ansi.h>
#include <string.h>
#include "local.h"
#include <stdint.h>

/* Copy a pointer-sized type at a time if aligned, otherwise byte-by-byte. */
typedef uintptr_t elemtype;

/* Nonzero if both X and Y are aligned on a "elemtype" boundary.  */
#define ALIGNED(X, Y) \
    (__is_aligned(X, sizeof(elemtype)) && __is_aligned(Y, sizeof(elemtype)))

/* How many bytes are copied each iteration of the 4X unrolled loop.  */
#define BIGBLOCKSIZE    (sizeof (elemtype) * 4)

/* How many bytes are copied each iteration of the word copy loop.  */
#define LITTLEBLOCKSIZE (sizeof (elemtype))

/* Threshhold for punting to the byte copier.  */
#define TOO_SMALL(LEN)  ((LEN) < BIGBLOCKSIZE)

#undef memcpy

void *
__inhibit_loop_to_libcall
memcpy (void *__restrict dst0,
	const void *__restrict src0,
	size_t len0)
{
  /* The byte-by-byte copy version cannot be used for CHERI since it does not
   * preserve tag bits. */
#if (defined(PREFER_SIZE_OVER_SPEED) || defined(__OPTIMIZE_SIZE__)) && !defined(__CHERI_PURE_CAPABILITY__)
  char *dst = (char *) dst0;
  char *src = (char *) src0;

  void *save = dst0;

  while (len0--)
    {
      *dst++ = *src++;
    }

  return save;
#else
  char *dst = dst0;
  const char *src = src0;
  elemtype *aligned_dst;
  const elemtype *aligned_src;

  /* If the size is small, or either SRC or DST is unaligned,
     then punt into the byte copy loop.  This should be rare.  */
  if (ALIGNED (src, dst))
    {
      aligned_dst = (elemtype*)dst;
      aligned_src = (elemtype*)src;

      /* Copy 4X elemtype words at a time if possible.  */
      while (len0 >= BIGBLOCKSIZE)
        {
          *aligned_dst++ = *aligned_src++;
          *aligned_dst++ = *aligned_src++;
          *aligned_dst++ = *aligned_src++;
          *aligned_dst++ = *aligned_src++;
          len0 -= BIGBLOCKSIZE;
        }

      /* Copy one elemtype word at a time if possible.  */
      while (len0 >= LITTLEBLOCKSIZE)
        {
          *aligned_dst++ = *aligned_src++;
          len0 -= LITTLEBLOCKSIZE;
        }

       /* Pick up any residual with a byte copier.  */
      dst = (char*)aligned_dst;
      src = (char*)aligned_src;
    }

  while (len0--)
    *dst++ = *src++;

  return dst0;
#endif /* not PREFER_SIZE_OVER_SPEED */
}

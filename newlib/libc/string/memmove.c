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
	<<memmove>>---move possibly overlapping memory

INDEX
	memmove

SYNOPSIS
	#include <string.h>
	void *memmove(void *<[dst]>, const void *<[src]>, size_t <[length]>);

DESCRIPTION
	This function moves <[length]> characters from the block of
	memory starting at <<*<[src]>>> to the memory starting at
	<<*<[dst]>>>. <<memmove>> reproduces the characters correctly
	at <<*<[dst]>>> even if the two areas overlap.


RETURNS
	The function returns <[dst]> as passed.

PORTABILITY
<<memmove>> is ANSI C.

<<memmove>> requires no supporting OS subroutines.

QUICKREF
	memmove ansi pure
*/

#include <string.h>
#include <_ansi.h>
#include <stddef.h>
#include <limits.h>
#include <stdint.h>
#include "local.h"

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

#undef memmove

/*SUPPRESS 20*/
void *
__inhibit_loop_to_libcall
memmove (void *dst_void,
	const void *src_void,
	size_t length)
{
  /* The byte-by-byte copy version cannot be used for CHERI since it does not
   * preserve tag bits. */
#if (defined(PREFER_SIZE_OVER_SPEED) || defined(__OPTIMIZE_SIZE__)) && !defined(__CHERI_PURE_CAPABILITY__)
    char *dst = dst_void;
  const char *src = src_void;

  if (src < dst && dst < src + length)
    {
      /* Have to copy backwards */
      src += length;
      dst += length;
      while (length--)
	{
	  *--dst = *--src;
	}
    }
  else
    {
      while (length--)
	{
	  *dst++ = *src++;
	}
    }

  return dst_void;
#else
  char *dst = dst_void;
  const char *src = src_void;
  elemtype *aligned_dst;
  const elemtype *aligned_src;

  if (src < dst && dst < src + length)
    {
      /* Destructive overlap...have to copy backwards */
      src += length;
      dst += length;
      if (ALIGNED (src, dst))
        {
          elemtype *aligned_dst = (elemtype*)dst;
          const  elemtype *aligned_src = (elemtype*)src;
          while (length >= LITTLEBLOCKSIZE)
            {

              *--aligned_dst = *--aligned_src;
              length -= LITTLEBLOCKSIZE;
            }
          /* Pick up any residual with a byte copier.  */
          dst = (char*)aligned_dst;
          src = (char*)aligned_src;
      }
      while (length--)
	{
	  *--dst = *--src;
	}
    }
  else
    {
      /* Use optimizing algorithm for a non-destructive copy to closely
         match memcpy. If the size is small or either SRC or DST is unaligned,
         then punt into the byte copy loop.  This should be rare.  */
      if (ALIGNED (src, dst))
        {
          aligned_dst = (elemtype*)dst;
          aligned_src = (elemtype*)src;

          /* Copy 4X elemtype words at a time if possible.  */
          while (length >= BIGBLOCKSIZE)
            {
              *aligned_dst++ = *aligned_src++;
              *aligned_dst++ = *aligned_src++;
              *aligned_dst++ = *aligned_src++;
              *aligned_dst++ = *aligned_src++;
              length -= BIGBLOCKSIZE;
            }

          /* Copy one elemtype word at a time if possible.  */
          while (length >= LITTLEBLOCKSIZE)
            {
              *aligned_dst++ = *aligned_src++;
              length -= LITTLEBLOCKSIZE;
            }

          /* Pick up any residual with a byte copier.  */
          dst = (char*)aligned_dst;
          src = (char*)aligned_src;
        }

      while (length--)
        {
          *dst++ = *src++;
        }
    }

  return dst_void;
#endif /* not PREFER_SIZE_OVER_SPEED */
}

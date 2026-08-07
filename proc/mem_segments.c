/* =========================================================================
 * Created on: <Tue Aug 04 18:02:52 +01 2026>
 * Time-stamp: <Tue Aug  4 18:04:24 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [file:../COPYING.gpl-v3] for details.
 * Desc      : ~/coding/c_prog/tlpi/proc/mem_segments.c -
 *
 * Listing 6.1: A program that does nothing in particular, but the
 * comments indicate which memory segments each type of variable is
 * allocated in.
 * ========================================================================= */
#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>

char globBuf[65536];         /* Uninitialized data segment */
int primes[] = {2, 3, 5, 7}; /* Initialized data segment */

static int square(int x) /* Allocated in frame for square() */
{
  int result; /* Allocated in frame for square() */

  result = x * x;
  return result; /* Return value passed via register */
}

static void doCalc(int val) /* Allocated in frame for doCalc() */
{
  printf("The square of %d is %d\n", val, square(val));

  if (val < 1000) {
    int t; /* Allocated in frame for doCalc() */

    t = val * val * val;
    printf("The cube of %d is %d\n", val, t);
  }
}

int main(int argc, char *argv[]) /* Allocated in frame for main() */
{
  static int key = 9973;      /* Initialized data segment */
  static char mbuf[10240000]; /* Uninitialized data segment */
  char *p;                    /* Allocated in frame for main() */

  p = malloc(1024); /* Points to memory in heap segment */

  doCalc(key);

  exit(EXIT_SUCCESS);
}

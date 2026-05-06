/* =========================================================================
 * Created on: <Tue May 05 14:43:06 +01 2026>
 * Time-stamp: <Tue May  5 15:20:20 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details
 * Desc      : ~/coding/c_prog/tlpi/threads/strerror_test.c -
 *
 * Listing 31.2: A program to test whether the implementation of
 * strerror() thread-safe. See [[file:strerror.c]]
 * DEPRECATED:WONTFIX: sys_nerr and sys_errlist are removed from glibc
 * 2.32 on.  This code won't compile.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h> /* Get declaration of strerror() */

static void *threadFunc(void *arg) {
  char *str;

  printf("Other thread about to call strerror()\n");
  str = strerror(EPERM);
  printf("Other thread: str (%p) = %s\n", str, str);

  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t t;
  int s;
  char *str;

  str = strerror(EINVAL);
  printf("Main thread has called strerror()\n");

  s = pthread_create(&t, NULL, threadFunc, NULL);
  if (s != 0)
    nmsysErr(s, "pthread_create");

  s = pthread_join(t, NULL);
  if (s != 0)
    nmsysErr(s, "pthread_join");

  /* If strerror() is not thread-safe, then the output of this printf() be
     the same as that produced by the analogous printf() in threadFunc() */

  printf("Main thread:  str (%p) = %s\n", str, str);

  exit(EXIT_SUCCESS);
}

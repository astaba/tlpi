/* =========================================================================
 * Created on: <Fri Apr 24 12:25:56 +01 2026>
 * Time-stamp: <Fri Apr 24 12:32:09 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/threads/detached_attrib.c -
 *
 * Listing 29.2: An example of the use of POSIX thread attributes
 * (pthread_attr_t): creating a detached thread.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h"	/* IWYU pragma: keep */
#include <pthread.h>

static void *threadFunc(void *x) { return x; }

int main(int argc, char *argv[]) {
  pthread_t thr;
  pthread_attr_t attr;
  int s;

  /* NOTE:(1) Init struct and Assigns default values */
  s = pthread_attr_init(&attr);
  if (s != 0)
    nmsysErr(s, "pthread_attr_init");
  /* NOTE:(2) Set specific value to detach right at creation. */
  s = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  if (s != 0)
    nmsysErr(s, "pthread_attr_setdetachstate");

  s = pthread_create(&thr, &attr, threadFunc, (void *)1);
  if (s != 0)
    nmsysErr(s, "pthread_create");

 /* NOTE:(3) No longer needed: destroy struct. */
  s = pthread_attr_destroy(&attr);
  if (s != 0)
    nmsysErr(s, "pthread_attr_destroy");

  s = pthread_join(thr, NULL);
  if (s != 0)
    nmsysErr(s, "pthread_join failed as expected");

  exit(EXIT_SUCCESS);
}

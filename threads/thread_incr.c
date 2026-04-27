/* =========================================================================
 * Created on: <Fri Apr 24 16:51:12 +01 2026>
 * Time-stamp: <Fri Apr 24 17:07:43 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/threads/thread_incr.c -
 *
 * Listing 30.1 This program employs two POSIX threads that increment
 * the same global variable, without using any synchronization
 * method. As a consequence, updates are sometimes lost.
 * See also [[file:thread_incr_mutex.c]].
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <pthread.h>

/* "volatile" prevents compiler optimizations of arithmetic
   operations on 'glob' */
static volatile int glob = 0;

/* Loop 'arg' times incrementing 'glob' */
static void *threadFunc(void *arg) {
  int loops = *((int *)arg);
  int loc, j;

  for (j = 0; j < loops; j++) {
    loc = glob;
    loc++;
    glob = loc;
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t t1, t2;
  int loops, s;

  loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;

  s = pthread_create(&t1, NULL, threadFunc, &loops);
  if (s != 0)
    nmsysErr(s, "pthread_create");
  s = pthread_create(&t2, NULL, threadFunc, &loops);
  if (s != 0)
    nmsysErr(s, "pthread_create");

  s = pthread_join(t1, NULL);
  if (s != 0)
    nmsysErr(s, "pthread_join");
  s = pthread_join(t2, NULL);
  if (s != 0)
    nmsysErr(s, "pthread_join");

  printf("glob = %d\n", glob);
  exit(EXIT_SUCCESS);
}

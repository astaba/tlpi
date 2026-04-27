/* =========================================================================
 * Created on: <Fri Apr 24 21:27:35 +01 2026>
 * Time-stamp: <Fri Apr 24 21:41:22 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/threads/thread_incr_mutex.c -
 *
 * Listing 30.2: This program employs two POSIX threads that increment
 * the same global variable, synchronizing their access using a mutex.
 * As a consequence, updates are not lost.
 * Compare with [[file:thread_incr.c]], [[file:thread_incr_spinlock.c]],
 * and [[file:thread_incr_rwlock.c]].
 * ========================================================================= */
#include "../lib/tlpi_hdr.h"	/* IWYU pragma: keep */
#include <pthread.h>

static volatile int glob = 0;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void * /* Loop 'arg' times incrementing 'glob' */
threadFunc(void *arg) {
  int loops = *((int *)arg);
  int loc, j, s;

  for (j = 0; j < loops; j++) {
    s = pthread_mutex_lock(&mtx);
    if (s != 0)
      nmsysErr(s, "pthread_mutex_lock");

    loc = glob;
    loc++;
    glob = loc;

    s = pthread_mutex_unlock(&mtx);
    if (s != 0)
      nmsysErr(s, "pthread_mutex_unlock");
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

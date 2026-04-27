/* =========================================================================
 * Created on: <Fri Apr 24 22:51:01 +01 2026>
 * Time-stamp: <Sun Apr 26 17:11:22 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/threads/ythread_incr_mutex.c -
 *
 * Listing 30.2y: This program employs two POSIX threads that increment
 * the same global variable, synchronizing their access using a mutex.
 * As a consequence, updates are not lost.
 * Compare with [[file:thread_incr.c]], [[file:thread_incr_spinlock.c]],
 * and [[file:thread_incr_rwlock.c]].
 * ========================================================================= */
#include "pthread_wrappers.h"

static volatile int glob = 0;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void *threadFunc(void *arg);

int main(int argc, char *argv[]) {
  pthread_t t1, t2;
  int loops;

  loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000L;

  Pthread_create(&t1, NULL, threadFunc, &loops);
  Pthread_create(&t2, NULL, threadFunc, &loops);

  Pthread_join(t1, NULL);
  Pthread_join(t2, NULL);

  printf("glob = %d\n", glob);
  exit(EXIT_SUCCESS);
}

/* Loop 'arg' times incrementing 'glob' */
static void *threadFunc(void *arg) {
  int loops = *((int *)arg);
  int loc, j;

  for (j = 0; j < loops; j++) {
    Pthread_mutex_lock(&mtx);
    loc = glob;
    loc++;
    glob = loc;
    Pthread_mutex_unlock(&mtx);
  }
  return NULL;
}

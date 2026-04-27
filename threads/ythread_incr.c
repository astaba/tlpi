/* =========================================================================
 * Created on: <Fri Apr 24 16:17:20 +01 2026>
 * Time-stamp: <Sun Apr 26 17:11:22 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/threads/ythread_incr.c -
 *
 * Listing 30.1y This program employs two POSIX threads that increment
 * the same global variable, without using any synchronization
 * method. As a consequence, updates are sometimes lost.
 * See also [[file:thread_incr_mutex.c]].
 * ========================================================================= */
#include "pthread_wrappers.h"

void *thread_routine(void *agr);

static volatile int glob = 0;

int main(int argc, char *argv[argc + 1]) {
  int iterNum;
  pthread_t tid1, tid2;

  iterNum = argc > 1 ? getInt(argv[1], GN_GT_0, "iters_num") : 10000000L;

  Pthread_create(&tid1, NULL, thread_routine, &iterNum);
  Pthread_create(&tid2, NULL, thread_routine, &iterNum);

  Pthread_join(tid1, NULL);
  Pthread_join(tid2, NULL);

  if (glob == 2 * iterNum)
    printf("%-5s: %d\n", "OK", glob);
  else
    printf("%-5s: %d\n", "BOOM!", glob);

  exit(EXIT_SUCCESS);
}

void *thread_routine(void *arg) {
  int iterNum = *((int *)arg);
  int i, loc;

  for (i = 0; i < iterNum; i++) {
    loc = glob;
    loc++;
    glob = loc;
  }
  return NULL;
}

/* =========================================================================
 * Created on: <Wed Apr 29 19:52:42 +01 2026>
 * Time-stamp: <Wed Apr 29 20:21:36 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/psem/ythread_incr_psem.c -
 *
 * Listing 53.6y: Use a POSIX unnamed semaphore to synchronize access
 * by two threads to a global variable.
 * See [[file:thread_incr_psem.c]],
 * [[file:../threads/ythread_incr.c]],
 * [[file:../threads/ythread_incr_mutex.c]].
 * ========================================================================= */
#include "../lib/pthread_wrappers.h"
#include "psem_wrappers.h"

static int glob = 0;
static sem_t sem;

static void *threadRoutine(void *arg) {
  int iterNum = *(int *)arg;
  int i, loc;

  for (i = 0; i < iterNum; i++) {
    Sem_Wait(&sem);
    loc = glob;
    loc++;
    glob = loc;
    Sem_Post(&sem);
  }

  return NULL;
}

int main(int argc, char *argv[argc + 1]) {
  int iterNum;
  pthread_t tid1, tid2;

  iterNum = (argc > 1) ? getInt(argv[1], GN_GT_0, "iter-num") : 1000000L;

  Sem_Init(&sem, 0, 1);

  Pthread_create(&tid1, NULL, threadRoutine, &iterNum);
  Pthread_create(&tid2, NULL, threadRoutine, &iterNum);

  Pthread_join(tid1, NULL);
  Pthread_join(tid2, NULL);

  if (glob == 2 * iterNum)
    printf("OK!:\t%d\n", glob);
  else
    printf("BOOM:\t%d\n", glob);

  exit(EXIT_SUCCESS);
}

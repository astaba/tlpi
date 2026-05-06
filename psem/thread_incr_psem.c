/* =========================================================================
 * Created on: <Wed Apr 29 18:33:10 +01 2026>
 * Time-stamp: <Wed Apr 29 21:35:21 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/psem/thread_incr_psem.c -
 *
 * Listing 53.6: Use a POSIX unnamed semaphore to synchronize access
 * by two threads to a global variable.
 * See also:
 * [[file:../threads/thread_incr.c]], [[file:../threads/thread_incr_mutex.c]].
 * [[file:../threads/ythread_incr.c]], [[file:../threads/ythread_incr_mutex.c]].
 * ========================================================================= */
#include"../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <pthread.h>
#include <semaphore.h>

static int glob = 0;
static sem_t sem;

static void * /* Loop 'arg' times incrementing 'glob' */
threadFunc(void *arg) {
  int loops = *((int *)arg);
  int loc, j;

  for (j = 0; j < loops; j++) {
    if (sem_wait(&sem) == -1)
      systmErr("sem_wait");

    loc = glob;
    loc++;
    glob = loc;

    if (sem_post(&sem) == -1)
      systmErr("sem_post");
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t t1, t2;
  int loops, s;

  loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;

  /* Initialize a semaphore with the value 1 */

  if (sem_init(&sem, 0, 1) == -1)
    systmErr("sem_init");

  /* Create two threads that increment 'glob' */

  s = pthread_create(&t1, NULL, threadFunc, &loops);
  if (s != 0)
    nmsysErr(s, "pthread_create");
  s = pthread_create(&t2, NULL, threadFunc, &loops);
  if (s != 0)
    nmsysErr(s, "pthread_create");

  /* Wait for threads to terminate */

  s = pthread_join(t1, NULL);
  if (s != 0)
    nmsysErr(s, "pthread_join");
  s = pthread_join(t2, NULL);
  if (s != 0)
    nmsysErr(s, "pthread_join");

  printf("glob = %d\n", glob);
  exit(EXIT_SUCCESS);
}

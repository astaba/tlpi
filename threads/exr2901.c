/* =========================================================================
 * Created on: <Fri Apr 24 12:56:05 +01 2026>
 * Time-stamp: <Fri Apr 24 15:03:00 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/threads/exr2901.c -
 *
 * Exercises 29.1.
 * What possible outcomes might there be if a thread executes the
 * following code:
 *	pthread_join(pthread_self(), NULL);
 *
 * Write a program to see what actually happens on Linux. If we have a
 * variable, tid, containing a thread ID, how can a thread prevent
 * itself from making a call, pthread_join(tid, NULL), that is
 * equivalent to the above statement?
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <pthread.h>

void *routine(void *arg);

int main(int argc, char *argv[argc + 1]) {
  int s;
  pthread_t tid1;

  // printf("Hello self thread!\n");
  // s = pthread_join(pthread_self(), NULL);
  // if (s != 0)
  //   nmsysErr(s, "pthread_join() failed");
  // /* NOTE: This is what happens right after the call: */
  // /* ERROR [EDEADLK/EDEADLOCK Resource deadlock avoided] pthread_join() failed */

  s = pthread_create(&tid1, NULL, routine, "Message from main thread!\n");
  if (s != 0)
    nmsysErr(s, "pthread_create() failed");

  /* NOTE: Passing tid reliably to a peer requires synchronization
     which is out of the scope of this exercise. */
  if (!pthread_equal(tid1, pthread_self())) {
    s = pthread_join(tid1, NULL);
    if (s != 0)
      nmsysErr(s, "pthread_join() failed");
  }

  exit(EXIT_SUCCESS);
}

void *routine(void *arg) {
  char *str = (char *)arg;
  printf("Peer Thread: '%s'", str);
  return NULL;
}

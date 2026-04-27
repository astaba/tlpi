/* =========================================================================
 * Created on: <Sat Apr 25 14:14:46 +01 2026>
 * Time-stamp: <Sun Apr 26 03:12:01 +01 2026 by owner>
 * Author    : Copyright(C) Michael Kerrisk, 2026
 *             See the file [[file:../COPYING.gpl-v3]] for details
 * Desc      : ~/coding/c_prog/tlpi/threads/prod_no_condvar.c -
 *             Supplementary program for Chapter 30
 *
 * A simple POSIX threads producer-consumer example that doesn't use a
 * condition variable.
 * See also [[file:prod_condvar.c]].
 * ========================================================================= */
#include "pthread_wrappers.h"
#include <stdbool.h>
#include <time.h>

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static int avail = 0;

static void *producer(void *arg) {
  int cnt = atoi((char *)arg);

  for (int j = 0; j < cnt; j++) {
    sleep(1);

    /* Code to produce a unit omitted */

    Pthread_mutex_lock(&mtx);
    avail++; /* Let consumer know another unit is available */
    Pthread_mutex_unlock(&mtx);
  }

  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc < 2)
    usageErr("%s <n> [ <n> ... ]\n"
             "For each arg a peer thread is created and passed\n"
             "<n> as the number of units to create while the\n"
             "main thread consumes all of them.\n",
             argv[0]);

  time_t t = time(NULL);
  /* Total number of units that all threads will produce */
  int totRequired = 0;

  /* Create all threads */
  for (int j = 1; j < argc; j++) {
    totRequired += atoi(argv[j]);
    pthread_t tid;
    Pthread_create(&tid, NULL, producer, argv[j]);
  }

  /* Use a polling loop to check for available units */

  int numConsumed = 0; /* Total units so far consumed */
  bool done = false;

  for (;;) {
    Pthread_mutex_lock(&mtx);

    while (avail > 0) { /* Consume all available units */
      /* Do something with produced unit */
      numConsumed++;
      avail--;
      printf("T=%ld: numConsumed=%d\n", (long)(time(NULL) - t), numConsumed);
      done = numConsumed >= totRequired;
    }

    Pthread_mutex_unlock(&mtx);

    if (done)
      break;
    /* Perhaps do other work here that does not require mutex lock */
  }

  exit(EXIT_SUCCESS);
}

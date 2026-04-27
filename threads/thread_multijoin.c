/* =========================================================================
 * Created on: <Sun Apr 26 03:31:50 +01 2026>
 * Time-stamp: <Sun Apr 26 16:38:27 +01 2026 by owner>
 * Author    : Copyright(C) Michael Kerrisk, 2026
 *             See the file [[file:../COPYING.gpl-v3]] for details
 * Desc      : ~/coding/c_prog/tlpi/threads/thread_multijoin.c -
 *
 * Listing 30.4:
 *
 * This program creates one thread for each of its command-line
 * arguments. Each thread sleeps for the number of seconds specified
 * in the corresponding command-line argument, and then
 * terminates. This sleep interval simulates "work" done by the
 * thread.
 *
 * The program maintains a global array (pointed to by 'thread')
 * recording information about all threads that have been created. The
 * items of this array record the thread ID ('tid') and current state
 * ('state', of type 'enum tstate') of each thread.
 *
 * As each thread terminates, it sets its 'state' to TS_TERMINATED and
 * signals the 'threadDied' condition variable. The main thread
 * continuously waits on this condition variable, and is thus informed
 * when any of the threads that it created has terminated. When
 * 'numLive', which records the number of live threads, falls to 0,
 * the main thread terminates.
 * ========================================================================= */
#include "pthread_wrappers.h"

/* Data type to name thread states */
enum tstate {
  TS_ALIVE,      /* Thread is alive */
  TS_TERMINATED, /* Thread terminated, not yet joined */
  TS_JOINED      /* Thread terminated, and joined */
};

/* NOTE: The following 2 variables are not shared. See:
   [[file:README.org::#main-thread-totthreads-and-numlive-global-variables]],
   [[file:README.org::#why-are-they-global-if-only-ever-used-by-main-thread]]
 */

static int totThreads = 0; /* Total number of threads created */
static int numLive = 0;    /* Total number of threads still alive or
                              terminated but not yet joined */

static pthread_cond_t threadDied = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;

/* WARNING: Protect following 2 shared variables. See:
   [[file:README.org::#mutex-important-general-principle]] */

/* Number of terminated threads that have not yet been joined */
static int numUnjoined = 0;

static struct {      /* Info about each thread */
  pthread_t tid;     /* ID of this thread */
  enum tstate state; /* Thread state (TS_* constants above) */
  int sleepTime;     /* Number seconds to live before terminating */
} *thread;           /* Instantiate pointer to anonymous struct (no tag) */

/* Start function for thread */
static void *threadFunc(void *arg) {
  int idx = (int)arg;

  /* NOTE: Shared data unprotected access? No. See:
   * [[file:README.org::#real-point-of-mutual-exclusion]] */
  sleep(thread[idx].sleepTime); /* Simulate doing some work */
  printf("Thread %d terminating\n", idx);

  Pthread_mutex_lock(&threadMutex);
  numUnjoined++;
  thread[idx].state = TS_TERMINATED;
  Pthread_mutex_unlock(&threadMutex);

  Pthread_cond_signal(&threadDied);

  return NULL;
}

int main(int argc, char *argv[]) {
  int idx;

  if (argc < 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s num-secs...\n", argv[0]);

  /* Created thread-monitor-array */
  thread = calloc(argc - 1, sizeof(*thread));
  if (thread == NULL)
    systmErr("calloc");

  /* Create all threads */
  for (idx = 0; idx < argc - 1; idx++) {
    thread[idx].sleepTime = getInt(argv[idx + 1], GN_NONNEG, NULL);
    thread[idx].state = TS_ALIVE;
    /* HACK: Fit int (32-bit) into pointer slot (64-bit). See:
     * [[file:README.org::#index-transmission-via-cast-is-contrived]] */
    Pthread_create(&thread[idx].tid, NULL, threadFunc, (void *)idx);
  }

  totThreads = argc - 1;
  numLive = totThreads;

  /* Zombie-Threads Joiner-Loop: Join with terminated threads */
  while (numLive > 0) {
    Pthread_mutex_lock(&threadMutex);

    while (numUnjoined == 0)
      Pthread_cond_wait(&threadDied, &threadMutex);

    /* Thread-Monitor-Array Walking-Loop */
    for (idx = 0; idx < totThreads; idx++) {
      if (thread[idx].state == TS_TERMINATED) {
        Pthread_join(thread[idx].tid, NULL);
        thread[idx].state = TS_JOINED;
        numLive--;
        numUnjoined--;
        printf("Reaped thread %d (numLive=%d)\n", idx, numLive);
      }
    }

    Pthread_mutex_unlock(&threadMutex);
  }

  /* NOTE: No malloc cleanup? See:
     [[file:README.org::#no-malloc-cleanup]] */
  exit(EXIT_SUCCESS);
}

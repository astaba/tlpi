/* =========================================================================
 * Created on: <Mon May 18 06:47:58 +01 2026>
 * Time-stamp: <Mon May 18 08:08:12 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/threads/ythread_multijoin.c -
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include "pthread_wrappers.h"

typedef enum {
  TS_ALIVE,
  TS_ZOMBIE,
  TS_JOINED,
} thrdstate_t;

typedef struct {
  pthread_t tid;
  thrdstate_t state;
  int sleepTime;
  int idx;
} thrds_t;

static void *thrdRoutine(void *arg);

int totalThrds;
int numAlive;
int numUnjoined = 0;
thrds_t *thrds;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int main(int argc, char *argv[argc + 1]) {
  time_t t;
  int j;

  if (argc == 1 || (argc > 1 && strcmp(argv[1], "--help") == 0))
    usageErr("%s <ns> [... ...]\n", argv[0]);

  thrds = calloc(argc - 1, sizeof(*thrds));
  if (thrds == NULL)
    systmErr("calloc() failed");

  t = time(NULL);

  for (j = 0; j < (argc - 1); j++) {
    thrds[j].sleepTime = getInt(argv[j + 1], GN_NONNEG, "sleept-time");
    thrds[j].state = TS_ALIVE;
    thrds[j].idx = j;
    Pthread_create(&thrds[j].tid, NULL, thrdRoutine, thrds + j);
  }
  /* for (j = 0; j < (argc - 1); j++) */
  /* Pthread_join(thrds[j].tid, NULL); */

  totalThrds = argc - 1;
  numAlive = totalThrds;

  while (numAlive > 0) {
    Pthread_mutex_lock(&mutex);

    while (numUnjoined == 0)
      Pthread_cond_wait(&cond, &mutex);

    for (j = 0; j < totalThrds; j++) {
      if (thrds[j].state == TS_ZOMBIE) {
        Pthread_join(thrds[j].tid, NULL);
        thrds[j].state = TS_JOINED;
        numUnjoined--;
        numAlive--;
        printf("Time=%ld: Joined thread n°%d\n", (long)time(NULL) - t,
               thrds[j].idx);
      }
    }

    Pthread_mutex_unlock(&mutex);
  }

  free(thrds);
  return EXIT_SUCCESS;
}

static void *thrdRoutine(void *arg) {
  int idx = ((thrds_t *)arg)->idx;
  int sleepTime = ((thrds_t *)arg)->sleepTime;

  sleep(sleepTime); /* Simulate unit production */
  printf("Thread[%d]: Terminating\n", idx);

  Pthread_mutex_lock(&mutex);
  ((thrds_t *)arg)->state = TS_ZOMBIE;
  numUnjoined++;
  Pthread_mutex_unlock(&mutex);

  Pthread_cond_signal(&cond);
  return NULL;
}

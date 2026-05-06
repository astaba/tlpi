/* =========================================================================
 * Created on: <Thu Apr 30 00:24:04 +01 2026>
 * Time-stamp: <Thu Apr 30 01:48:52 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/psem/ypsem_timedwait.c -
 *
 * See man sem_timedwait(3) example program.
 * ========================================================================= */
#include "../lib/get_num.h"
#include <errno.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define handle_error(msg)                                                      \
  do {                                                                         \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

static sem_t sem;

/* Only call async-signal-safe functions */
static void handler(int signum) {
  write(STDOUT_FILENO, "handler: sem_post()\n", 20);
  if (sem_post(&sem) == -1) {
    write(STDERR_FILENO, "handler: sem_post() failed\n", 27);
    _exit(EXIT_FAILURE);
  }
}

int main(int argc, char *argv[argc + 1]) {
  int rc;
  struct sigaction sa;
  unsigned int alarm_secs, wait_secs;
  struct timespec ts;

  if (argc != 3) {
    fprintf(stderr, "%s <alarm-secs> <wait-secs>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (sem_init(&sem, 0, 0) == -1)
    handle_error("sem_init() failed");

  /* Register SIGALRM handler */
  sa.sa_handler = handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGALRM, &sa, NULL) == -1)
    handle_error("sigaction() failed");

  /* Set alarm time using argv[1] */
  alarm_secs = getInt(argv[1], GN_GT_0, "alarm_secs");
  alarm(alarm_secs);

  /* Compute relative time as current time plus argv[2] */
  if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    handle_error("clock_gettime() failed");
  wait_secs = getInt(argv[2], GN_GT_0, "wait_secs");
  ts.tv_sec += wait_secs;

  printf("%s() about to call sem_timedwait()\n", __func__);
  while ((rc = sem_timedwait(&sem, &ts)) == -1 && errno == EINTR)
    continue;

  /* Check what happened */
  if (rc == -1) {
    if (errno == ETIMEDOUT)
      printf("sem_timedwait() timed out\n");
    else
      perror("sem_timedwait() failed");
  } else
    printf("sem_timedwait() succeeded\n");

  exit((rc == 0) ? EXIT_SUCCESS : EXIT_FAILURE);
}

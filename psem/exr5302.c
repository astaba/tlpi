/* =========================================================================
 * Created on: <Thu Apr 30 01:56:05 +01 2026>
 * Time-stamp: <Fri May  1 13:37:07 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/psem/exr5302.c -
 *
 * Exercise 53.2
 * Modify the program in [[file:psem_wait.c][Listing 53.3]] to use
 * sem_timedwait() instead of sem_wait(). The program should take an
 * additional command-line argument that specifies a (relative) number
 * of seconds to be used as the timeout for the sem_timedwait() call.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <semaphore.h>
#include <time.h>

int main(int argc, char *argv[]) {
  sem_t *sem;
  struct timespec ts;
  unsigned int wait_secs;
  int rc;

  if (argc < 3 || strcmp(argv[1], "--help") == 0)
    usageErr("%s <sem-name> <wait-secs>\n", argv[0]);

  sem = sem_open(argv[1], 0);
  if (sem == SEM_FAILED)
    systmErr("sem_open");

  if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
    systmErr("clock_gettime() failed");
  wait_secs = getInt(argv[2], GN_NONNEG, "wait_secs");
  /* [[file:README.org::#casting-time-types]] */
  ts.tv_sec += (time_t)wait_secs;

  /* [[file:README.org::#absolute-time-and-eintr-loop]] */
  while ((rc = sem_timedwait(sem, &ts)) == -1 && errno == EINTR)
    continue;

  if (rc == -1) {
    if (errno == ETIMEDOUT)
      custmErr("%ld sem_timedwait() timed out\n", (long)getpid());
    else
      systmErr("sem_timedwait() failed");
  }

  printf("%ld sem_timedwait() succeeded\n", (long)getpid());
  exit(EXIT_SUCCESS);
}

/* =========================================================================
 * Created on: <Wed Apr 29 15:14:13 +01 2026>
 * Time-stamp: <Wed Apr 29 15:18:13 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/psem/psem_wait.c -
 *
 * Listing 53.3
 * Decrease the value of a POSIX named semaphore.
 * See also [[file:psem_post.c]].
 * On Linux, named semaphores are supported with kernel 2.6 or later,
 * and a glibc that provides the NPTL threading implementation.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <semaphore.h>

int main(int argc, char *argv[]) {
  sem_t *sem;

  if (argc < 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s sem-name\n", argv[0]);

  sem = sem_open(argv[1], 0);
  if (sem == SEM_FAILED)
    systmErr("sem_open");

  if (sem_wait(sem) == -1)
    systmErr("sem_wait");

  printf("%ld sem_wait() succeeded\n", (long)getpid());
  exit(EXIT_SUCCESS);
}

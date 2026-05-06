/* =========================================================================
 * Created on: <Wed Apr 29 15:45:21 +01 2026>
 * Time-stamp: <Wed Apr 29 15:47:20 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/psem/psem_post.c -
 *
 * Listing 53.4: Increase the value of a POSIX named semaphore.
 * See also [[file:psem_wait.c]].
 *
 * On Linux, named semaphores are supported with kernel 2.6 or later,
 * and a glibc that provides the NPTL threading implementation.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <semaphore.h>

int main(int argc, char *argv[]) {
  sem_t *sem;

  if (argc != 2)
    usageErr("%s sem-name\n", argv[0]);

  sem = sem_open(argv[1], 0);
  if (sem == SEM_FAILED)
    systmErr("sem_open");

  if (sem_post(sem) == -1)
    systmErr("sem_post");
  exit(EXIT_SUCCESS);
}

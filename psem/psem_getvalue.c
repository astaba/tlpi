/* =========================================================================
 * Created on: <Wed Apr 29 16:00:17 +01 2026>
 * Time-stamp: <Wed Apr 29 16:01:42 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/psem/psem_getvalue.c -
 *
 * Listing 53.5: Obtain the value of a POSIX named semaphore.
 *
 * On Linux, named semaphores are supported with kernel 2.6 or later,
 * and a glibc that provides the NPTL threading implementation.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <semaphore.h>

int main(int argc, char *argv[]) {
  int value;
  sem_t *sem;

  if (argc != 2)
    usageErr("%s sem-name\n", argv[0]);

  sem = sem_open(argv[1], 0);
  if (sem == SEM_FAILED)
    systmErr("sem_open");

  if (sem_getvalue(sem, &value) == -1)
    systmErr("sem_getvalue");

  printf("%d\n", value);
  exit(EXIT_SUCCESS);
}

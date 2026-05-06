/* =========================================================================
 * Created on: <Wed Apr 29 14:47:39 +01 2026>
 * Time-stamp: <Wed Apr 29 14:51:38 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/psem/psem_unlink.c -
 *
 * Listing 53-2 Unlink a POSIX named semaphore.
 * On Linux, named semaphores are supported with kernel 2.6 or later,
 * and a glibc that provides the NPTL threading implementation.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <semaphore.h>

int main(int argc, char *argv[]) {
  if (argc != 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s sem-name\n", argv[0]);

  if (sem_unlink(argv[1]) == -1)
    systmErr("sem_unlink");

  exit(EXIT_SUCCESS);
}

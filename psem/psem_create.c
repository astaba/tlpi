/* =========================================================================
 * Created on: <Tue Apr 28 01:02:45 +01 2026>
 * Time-stamp: <Wed Apr 29 14:11:26 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/psem/psem_create.c -
 *
 * Listing 53.1:
 * Create a POSIX named semaphore.
 * Usage as shown in usageError().
 * On Linux, named semaphores are supported with kernel 2.6 or later,
 * and a glibc that provides the NPTL threading implementation.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>

static void usageError(const char *cmd) {
  fprintf(stderr, "Usage: %s [-cx] name [octal-perms [value]]\n", cmd);
  fprintf(stderr, "    -c   Create semaphore (O_CREAT)\n");
  fprintf(stderr, "    -x   Create exclusively (O_EXCL)\n");
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  int flags, opt;
  mode_t perms;
  unsigned int value;
  sem_t *sem;

  flags = 0;
  while ((opt = getopt(argc, argv, "cx")) != -1) {
    switch (opt) {
    case 'c':
      flags |= O_CREAT;
      break;
    case 'x':
      flags |= O_EXCL;
      break;
    default:
      usageError(argv[0]);
    }
  }
  /* optind value is one more than the last index successfully parsed. */
  if (optind >= argc) /* There were flag(s) but no required argument */
    usageError(argv[0]);
  /* At this point optind holds required arg index */

  /* Default permissions are rw-------;
     default semaphore initialization value is 0 */

  /* Is the second optional argument missing? */
  perms = (optind + 1 >= argc)
              ? (S_IRUSR | S_IWUSR)
              : getInt(argv[optind + 1], GN_BASE_8, "octal-perms");
  /* Is the third optional argument missing? */
  value = (optind + 2 >= argc) ? 0 : getInt(argv[optind + 2], 0, "value");

  sem = sem_open(argv[optind], flags, perms, value);
  if (sem == SEM_FAILED)
    systmErr("sem_open");

  exit(EXIT_SUCCESS);
}

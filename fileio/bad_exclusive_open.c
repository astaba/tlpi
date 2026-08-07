/* =========================================================================
 * Created on: <Wed Jun 17 19:02:03 +01 2026>
 * Time-stamp: <Wed Jun 17 19:11:16 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/fileio/bad_exclusive_open.c -
 *
 * Listing 5.1:
 * The following code shows why we need the open() O_EXCL flag.
 *
 * This program tries to ensure that it is the one that creates the
 * file named in its command-line argument. It does this by trying to
 * open() the filename once without the O_CREAT flag (if this open()
 * succeeds then the program know it is not the creator of the file),
 * and if that open() fails, it calls open() a second time, with the
 * O_CREAT flag.
 *
 * If the first open() fails, the program assumes that it is the
 * creator of the file. However this may not be true: some other
 * process may have created the file between the two calls to open().
 * ========================================================================= */
#include "tlpi_hdr.h" // IWYU pragma: keep
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  int fd;

  if (argc < 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s file\n", argv[0]);

  fd = open(argv[1], O_WRONLY); /* Open 1: check if file exists */
  if (fd != -1) {               /* Open succeeded */
    printf("[PID %ld] File \"%s\" already exists\n", (long)getpid(), argv[1]);
    close(fd);
  } else {
    if (errno != ENOENT) { /* Failed for unexpected reason */
      systmErr("open");
    } else {
      printf("[PID %ld] File \"%s\" doesn't exist yet\n", (long)getpid(),
             argv[1]);
      if (argc > 2) { /* Delay between check and create */
        sleep(5);     /* Suspend execution for 5 seconds */
        printf("[PID %ld] Done sleeping\n", (long)getpid());
      }
      fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
      if (fd == -1)
        systmErr("open");

      printf("[PID %ld] Created file \"%s\" exclusively\n", (long)getpid(),
             argv[1]); /* BUG: MAY NOT BE TRUE! */
    }
  }

  exit(EXIT_SUCCESS);
}

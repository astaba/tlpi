/* =========================================================================
 * Created on: <Thu Aug 06 16:20:44 +01 2026>
 * Time-stamp: <Thu Aug  6 16:21:33 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [file:../COPYING.gpl-v3] for details.
 * Desc      : ~/coding/c_prog/tlpi/procexec/fork_file_sharing.c -
 *
 * Listing 24.2: Show that the file descriptors of a forked child
 * refer to the same open file objects as the parent.
 * ========================================================================= */
#include "tlpi_hdr.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  int fd, flags;
  char template[] = "/tmp/testXXXXXX";

  setbuf(stdout, NULL); /* Disable buffering of stdout */

  /* Open a temporary file, set its file offset to some arbitrary value,
     and change the setting of one of the open file status flags. */

  fd = mkstemp(template);
  if (fd == -1)
    systmErr("mkstemp");

  printf("File offset before fork(): %lld\n",
         (long long)lseek(fd, 0, SEEK_CUR));

  flags = fcntl(fd, F_GETFL);
  if (flags == -1)
    systmErr("fcntl - F_GETFL");
  printf("O_APPEND flag before fork() is: %s\n",
         (flags & O_APPEND) ? "on" : "off");

  switch (fork()) {
  case -1:
    systmErr("fork");

  case 0: /* Child: change file offset and status flags */
    if (lseek(fd, 1000, SEEK_SET) == -1)
      systmErr("lseek");

    flags = fcntl(fd, F_GETFL); /* Fetch current flags */
    if (flags == -1)
      systmErr("fcntl - F_GETFL");
    flags |= O_APPEND; /* Turn O_APPEND on */
    if (fcntl(fd, F_SETFL, flags) == -1)
      systmErr("fcntl - F_SETFL");
    _exit(EXIT_SUCCESS);

  default: /* Parent: can see file changes made by child */
    if (wait(NULL) == -1)
      systmErr("wait"); /* Wait for child exit */
    printf("Child has exited\n");

    printf("File offset in parent: %lld\n", (long long)lseek(fd, 0, SEEK_CUR));

    flags = fcntl(fd, F_GETFL);
    if (flags == -1)
      systmErr("fcntl - F_GETFL");
    printf("O_APPEND flag in parent is: %s\n",
           (flags & O_APPEND) ? "on" : "off");
    exit(EXIT_SUCCESS);
  }
}

/* =========================================================================
 * Created on: <Wed Jun 17 19:13:58 +01 2026>
 * Time-stamp: <Sun Jun 21 20:56:45 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/fileio/large_file.c -
 *
 * Listing 5.3:
 * NOTICE: This usage of _LARGEFILE64_SOURCE is outdated,
 * see [[file:ylarge_file.c]]
 * Demonstrate the use of the (obsolete) Large File System API.
 * WARN: This program is Linux-specific.
 * ========================================================================= */
#define _LARGEFILE64_SOURCE
#include "tlpi_hdr.h" /* IWYU pragma: keep */
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  int fd;
  off64_t off;

  if (argc != 3 || strcmp(argv[1], "--help") == 0)
    usageErr("%s pathname offset\n", argv[0]);

  fd = open64(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if (fd == -1)
    systmErr("open64");

  off = atoll(argv[2]);
  if (lseek64(fd, off, SEEK_SET) == -1)
    systmErr("lseek64");

  if (write(fd, "test", 4) == -1)
    systmErr("write");
  exit(EXIT_SUCCESS);
}

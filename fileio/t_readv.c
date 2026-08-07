/* =========================================================================
 * Created on: <Wed Jun 17 19:11:58 +01 2026>
 * Time-stamp: <Wed Jun 17 19:13:34 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/fileio/t_readv.c -
 *
 * Listing 5.2: Demonstrate the use of the readv() system call to
 * perform "gather I/O".
 *
 * (This program is merely intended to provide a code snippet for the
 * book; unless you construct a suitably formatted input file, it
 * can't be usefully executed.)
 * ========================================================================= */
#include "tlpi_hdr.h" /* IWYU pragma: keep */
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/uio.h>

int main(int argc, char *argv[]) {
  int fd;
  struct iovec iov[3];
  struct stat myStruct; /* First buffer */
  int x;                /* Second buffer */
#define STR_SIZE 100
  char str[STR_SIZE]; /* Third buffer */
  ssize_t numRead, totRequired;

  if (argc != 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s file\n", argv[0]);

  fd = open(argv[1], O_RDONLY);
  if (fd == -1)
    systmErr("open");

  totRequired = 0;

  iov[0].iov_base = &myStruct;
  iov[0].iov_len = sizeof(struct stat);
  totRequired += iov[0].iov_len;

  iov[1].iov_base = &x;
  iov[1].iov_len = sizeof(x);
  totRequired += iov[1].iov_len;

  iov[2].iov_base = str;
  iov[2].iov_len = STR_SIZE;
  totRequired += iov[2].iov_len;

  numRead = readv(fd, iov, 3);
  if (numRead == -1)
    systmErr("readv");

  if (numRead < totRequired)
    printf("Read fewer bytes than requested\n");

  printf("total bytes requested: %ld; bytes read: %ld\n", (long)totRequired,
         (long)numRead);
  exit(EXIT_SUCCESS);
}

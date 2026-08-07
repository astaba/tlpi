/* =========================================================================
 * Created on: <Wed Jun 17 19:04:09 +01 2026>
 * Time-stamp: <Wed Jun 17 19:06:27 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/fileio/copy.c -
 *
 * Listing 4.1:
 * Copy the file named argv[1] to a new file named in argv[2].
 * ========================================================================= */
#include "tlpi_hdr.h" // IWYU pragma: keep
#include <fcntl.h>
#include <sys/stat.h>

#ifndef BUF_SIZE /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif

int main(int argc, char *argv[]) {
  int inputFd, outputFd, openFlags;
  mode_t filePerms;
  ssize_t numRead;
  char buf[BUF_SIZE];

  if (argc != 3 || strcmp(argv[1], "--help") == 0)
    usageErr("%s old-file new-file\n", argv[0]);

  /* Open input and output files */

  inputFd = open(argv[1], O_RDONLY);
  if (inputFd == -1)
    systmErr("opening file %s", argv[1]);

  openFlags = O_CREAT | O_WRONLY | O_TRUNC;
  /* rw-rw-rw- */
  filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  outputFd = open(argv[2], openFlags, filePerms);
  if (outputFd == -1)
    systmErr("opening file %s", argv[2]);

  /* Transfer data until we encounter end of input or an error */

  while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
    if (write(outputFd, buf, numRead) != numRead)
      custmErr("write() returned error or partial write occurred");
  if (numRead == -1)
    systmErr("read");

  if (close(inputFd) == -1)
    systmErr("close input");
  if (close(outputFd) == -1)
    systmErr("close output");

  exit(EXIT_SUCCESS);
}

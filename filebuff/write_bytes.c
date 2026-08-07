/* =========================================================================
 * Created on: <Fri Jul 10 13:21:21 +01 2026>
 * Time-stamp: <Sat Jul 11 11:06:16 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/filebuff/write_bytes.c -
 *
 * Supplementary program for Chapter 13: Write bytes to a file.
 * (A simple program for file I/O benchmarking.)
 *
 * Usage: write_bytes file num-bytes buf-size
 *
 * Writes 'num-bytes' bytes to 'file', using a buffer size of
 * 'buf-size' for each write().
 *
 * If compiled with -DUSE_O_SYNC, open the file with the O_SYNC flag,
 * so that all data and metadata changes are flushed to the disk.
 *
 * If compiled with -DUSE_FDATASYNC, perform an fdatasync() after each
 * write, so that data--and possibly metadata--changes are flushed to
 * the disk.
 *
 * If compiled with -DUSE_FSYNC, perform an fsync() after each write,
 * so that data and metadata are flushed to the disk.
 * ========================================================================= */
#include "tlpi_hdr.h" 
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  if (argc != 4 || strcmp(argv[1], "--help") == 0)
    usageErr("%s file num-bytes buf-size\n", argv[0]);

  size_t numBytes = getLong(argv[2], GN_GT_0, "num-bytes");
  size_t bufSize = getLong(argv[3], GN_GT_0, "buf-size");

  char *buf = malloc(bufSize);
  if (buf == NULL)
    systmErr("malloc");

  int openFlags = O_CREAT | O_WRONLY;

#if defined(USE_O_SYNC) && defined(O_SYNC)
  openFlags |= O_SYNC;
#endif

  int fd = open(argv[1], openFlags, S_IRUSR | S_IWUSR);
  if (fd == -1)
    systmErr("open");

  size_t thisWrite, totWritten;
  for (totWritten = 0; totWritten < numBytes; totWritten += thisWrite) {
    thisWrite = min(bufSize, numBytes - totWritten);

    if (write(fd, buf, thisWrite) != (ssize_t)thisWrite)
      custmErr("partial/failed write");

#ifdef USE_FSYNC
    if (fsync(fd))
      systmErr("fsync");
#endif
#ifdef USE_FDATASYNC
    if (fdatasync(fd))
      systmErr("fdatasync");
#endif
  }

  if (close(fd) == -1)
    systmErr("close");
  exit(EXIT_SUCCESS);
}

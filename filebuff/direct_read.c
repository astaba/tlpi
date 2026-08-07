/* =========================================================================
 * Created on: <Fri Jul 10 13:28:58 +01 2026>
 * Time-stamp: <Wed Aug  5 10:34:14 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/filebuff/direct_read.c -
 *
 * Listing 13.1: Demonstrate the use of O_DIRECT to perform I/O
 * bypassing the buffer cache ("direct I/O").
 *
 * Usage: direct_read file length [offset [alignment]]
 *
 * WARN: This program is Linux-specific.
 * ========================================================================= */
#define _GNU_SOURCE   /* Obtain O_DIRECT definition from <fcntl.h> */
#include "tlpi_hdr.h" /* IWYU pragma: keep */
#include <fcntl.h>
#include <malloc.h>

int main(int argc, char *argv[]) {
  int fd;
  ssize_t numRead;
  size_t length, alignment;
  off_t offset;
  char *buf;

  /* Validate args */
  if (argc < 3 || strcmp(argv[1], "--help") == 0)
    usageErr("%s file length [offset [alignment]]\n", argv[0]);
  /* Parse args */
  length = getLong(argv[2], GN_ANY_BASE, "length");
  offset = (argc > 3) ? getLong(argv[3], GN_ANY_BASE, "offset") : 0;
  alignment = (argc > 4) ? getLong(argv[4], GN_ANY_BASE, "alignment") : 4096;

  /* Open disk file */
  fd = open(argv[1], O_RDONLY | O_DIRECT);
  if (fd == -1)
    systmErr("open");

  /* NOTE: memalign() allocates a block of memory aligned on an
     address that is a multiple of its first argument. By specifying
     this argument as 2 * 'alignment' and then adding 'alignment' to
     the returned pointer, we ensure that 'buf' is aligned on an odd
     multiple of 'alignment'. We do this to ensure that if, for
     example, we ask for a 256-byte aligned buffer, we don't
     accidentally get a buffer that is also aligned on a 512-byte
     boundary. */

  buf = memalign(alignment * 2, length + alignment);
  if (buf == NULL)
    systmErr("memalign");

  buf += alignment;

  if (lseek(fd, offset, SEEK_SET) == -1)
    systmErr("lseek");

  /* NOTE: Because of Direct I/O if the offset value in the previous
     lseek() call doesn't match a block size multiple, the next read()
     system call will throw an exception as EINVAL stemming from an
     offset mis-alignment in fd argument */

  numRead = read(fd, buf, length);
  if (numRead == -1)
    systmErr("read");
  printf("Read %ld bytes\n", (long)numRead);

  exit(EXIT_SUCCESS);
}

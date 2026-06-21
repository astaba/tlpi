/* =========================================================================
 * Created on: <Sat Jun 13 22:57:36 +01 2026>
 * Time-stamp: <Sat Jun 13 23:05:30 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 * Desc      : ~/coding/c_prog/tlpi/fileio/snippet0402.c -
 *
 * Listing 4.2 : Examples of the use of open()
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" // IWYU pragma: keep
#include <fcntl.h>

void sanbox(void) {
  int fd;

  /* Open existing file for reading */
  fd = open("startup", O_RDONLY);
  if (fd == -1)
    systmErr("open");

  /* Open new or existing file for reading and writing, truncating to
     zero bytes; file permissions read+write for owner, nothing for
     all others */
  fd = open("myfile", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (fd == -1)
    systmErr("open");

  /* Open new or existing file for writing; writes should always
     append to end of file */
  fd =
      open("w.log", O_WRONLY | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR);
  if (fd == -1)
    systmErr("open");
}

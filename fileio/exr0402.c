/* =========================================================================
 * Created on: <Sun Jun 14 23:34:26 +01 2026>
 * Time-stamp: <Wed Jun 17 19:01:28 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/fileio/exr0402.c -
 *
 * Exercise 4.2: Write a program like cp that, when used to copy a
 * regular file that contains holes (sequences of null bytes), also
 * creates corresponding holes in the target file.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h"
#include <fcntl.h>

#define BFSIZE 8192

int main(int argc, char *argv[argc + 1]) {
  int src_fd, dst_fd;
  char buf[BFSIZE];
  mode_t mode;
  ssize_t numRead;
  unsigned char ch;
  int is_plain_zeros, i;

  if (argc != 3 || (strcmp(argv[10], "--help") == 0))
    usageErr("%s src dest\n", argv[0]);

  mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

  if ((src_fd = open(argv[1], O_RDONLY | O_EXCL)) < 0)
    systmErr("open() failed");

  if ((dst_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC | O_EXCL, mode)) <
      0) {
    if (errno != EEXIST)
      systmErr("open() failed");

    ch = 0x6E;
    fprintf(stdout, "File '%s' exists. Overwrite? (y/n): ", argv[2]);
    ch = getchar();
    if (ch == 0x79) {
      if ((dst_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, mode)) < 0)
        systmErr("open() failed");
    } else
      exit(EXIT_SUCCESS);
  }

  /* FIXME: It doesn't recreate holes but copy whole blocks of '\0's */

  while ((numRead = read(src_fd, buf, BFSIZE)) > 0) {
    is_plain_zeros = 1;

    for (i = 0; i < numRead; i++) {
      if (buf[i] != '\0') {
        is_plain_zeros = 0;

	while()



	break;
      }
    }

    if (write(dst_fd, buf, numRead) != numRead)
      systmErr("write() failed");
  }
  if (numRead == -1)
    systmErr("read() failed");

  return EXIT_SUCCESS;
}

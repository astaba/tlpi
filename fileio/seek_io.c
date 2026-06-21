/* =========================================================================
 * Created on: <Sat Jun 13 23:18:10 +01 2026>
 * Time-stamp: <Wed Jun 17 19:18:16 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/fileio/seek_io.c -
 *
 * Listing 4.3:
 * Demonstrate the use of lseek() and file I/O system calls.
 *
 * Usage: seek_io file {r<length>|R<length>|w<string>|s<offset>}...
 *
 * This program opens the file named on its command line, and then
 * performs the file I/O operations specified by its remaining
 * command-line arguments:
 *
 * r<length> Read 'length' bytes from the file at current file offset,
 *           displaying them as text.
 * R<length> Read 'length' bytes from the file at current file offset,
 *           displaying them in hex.
 * w<string> Write 'string' at current file offset.
 * s<offset> Set the file offset to 'offset'.
 *
 * Example:
 *	     $ ./seek_io myfile wxyz s1 r2
 *
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  size_t len;
  off_t offset;
  mode_t filePerms;
  int fd, ap, j;
  unsigned char *buf;
  ssize_t numRead, numWritten;

  if (argc < 3 || strcmp(argv[1], "--help") == 0)
    usageErr("%s file {r<length>|R<length>|w<string>|s<offset>}...\n", argv[0]);

   /* rw-rw-rw- */
  filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  fd = open(argv[1], O_RDWR | O_CREAT, filePerms);
  if (fd == -1)
    systmErr("open");

  for (ap = 2; ap < argc; ap++) {
    switch (argv[ap][0]) {
    case 'r': /* Display bytes at current offset, as text */
    case 'R': /* Display bytes at current offset, in hex */
      len = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);

      buf = malloc(len);
      if (buf == NULL)
        systmErr("malloc");

      numRead = read(fd, buf, len);
      if (numRead == -1)
        systmErr("read");

      if (numRead == 0) {
        printf("%s: end-of-file\n", argv[ap]);
      } else {
        printf("%s: ", argv[ap]);
        for (j = 0; j < numRead; j++) {

          if (argv[ap][0] == 'r')
            printf("%c", isprint(buf[j]) ? buf[j] : '?');
          else
            printf("%02x ", buf[j]);
        }
        printf("\n");
      }

      free(buf);
      break;

    case 'w': /* Write string at current offset */
      numWritten = write(fd, &argv[ap][1], strlen(&argv[ap][1]));
      if (numWritten == -1)
        systmErr("write");
      printf("%s: wrote %ld bytes\n", argv[ap], (long)numWritten);
      break;

    case 's': /* Change file offset */
      offset = getLong(&argv[ap][1], GN_ANY_BASE, argv[ap]);
      if (lseek(fd, offset, SEEK_SET) == -1)
        systmErr("lseek");
      printf("%s: seek succeeded\n", argv[ap]);
      break;

    default:
      cmdLineErr("Argument must start with [Rrsw]: %s\n", argv[ap]);
    }
  }

  if (close(fd) == -1)
    systmErr("close");

  exit(EXIT_SUCCESS);
}

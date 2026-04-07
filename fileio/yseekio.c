/* Created on : Fri Jan 30 05:15:00 +01 2026 */
/* Description: Listing 4-3 practice draft. */
/* Purpose    : Demonstrate the use of lseek(2) in conjunction with read(2) and
                write(2) */
/* Options list: */
/* soffset: Seek to byte offset from the start of the file. */
/* rlength: Read length bytes from the file, starting at the current file
 *          offset, and display them in text form. */
/* Rlength: Read length bytes from the file, starting at the current file
 *          offset, and display them in hexadecimal. */
/* wstr:    Write the string of characters specified in str at the current file
 *          offset. */
/* WARN: For the experimental test: */
/* Only pass one I/O option per call after s<offset> */
/*
  $ touch tfile                  # Create new, empty file
  $ ./seek_io tfile s100000 wabc # Seek to offset 100,000, write “abc”
  $ ls -l tfile                  # Check size of file
  $ ./seek_io tfile s10000 R5    # Seek offset 10,000, read 5 bytes from hole
*/

#include "../lib/tlpi_hdr.h" // IWYU pragma: keep
#include <ctype.h>
#include <fcntl.h>

static inline int is_help(const char *h) {
  return (strcmp(h, "--help") == 0) || (strcmp(h, "-h") == 0);
}

int main(int argc, char *argv[argc + 1]) {
  /* Declare stack arguments here */
  size_t len;
  off_t offset;
  int fd, i, j;
  unsigned char *buf;
  ssize_t numRead, numWritten;
  unsigned int index = 0;

  /* Validate arguments number */
  if (argc < 3 || is_help(argv[1]))
    usageErr("%s filename {s<offset>|r<length>|R<length>|w<str>}\n", argv[0]);

  /* Open binary stream */
  fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd == -1)
    systmErr("open() failed");

  for (i = 2; i < argc; i++) {
    switch (argv[i][0]) {
    case 'R': /* Display bytes at current offset as hexadecimal */
    case 'r': /* Display bytes at current offset as text */
      len = getLong(&argv[i][1], GN_ANY_BASE, argv[i]);
      buf = malloc(sizeof(char) * len);
      if (buf == NULL)
        systmErr("malloc() failed");

      if ((numRead = read(fd, buf, len)) == -1)
        systmErr("read() failed");

      if (numRead == 0) {
        printf("%s: EOF nothing to read.\n", argv[i]);
      } else {
        printf("%s: ", argv[i]);
        if (argv[i][0] == 'r') {
          for (j = 0; j < numRead; j++) {
            printf("%c", isprint(buf[j]) ? buf[j] : '?');
          }
        } else {
          printf("\n\t0x%04x: ", index);
          for (j = 0; j < numRead; j++) {
            index++;
            printf("%02x ", buf[j]);
            if ((j + 1) % 8 == 0)
              printf("\n\t0x%04x: ", index);
          }
        }
        printf("\n");
      }
      free(buf);
      break;

    case 'w': /* Write string at current offset */
      numWritten = write(fd, &argv[i][1], (size_t)strlen(&argv[i][1]));
      if (numWritten == -1)
        systmErr("write() failed");
      printf("%s: Wrote %ld byte(s) of '%.*s'\n", argv[1], (long)numWritten,
             (int)numWritten, &argv[i][1]);
      break;

    case 's': /* Change file offset */
      offset = getLong(&argv[i][1], GN_ANY_BASE, argv[i]);
      if (lseek(fd, offset, SEEK_SET) == -1)
        systmErr("lseek() failed");
      printf("%s: offset %ld\n", argv[i], (long)offset);
      break;

    default:
      cmdLineErr("All options must start with either [Rrsw]: %s\n", argv[i]);
    }
  }

  if (close(fd) == -1)
    systmErr("close() failed");

  return EXIT_SUCCESS;
}

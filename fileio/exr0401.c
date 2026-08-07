/* =========================================================================
 * Created on: <Sun Jun 14 18:53:24 +01 2026>
 * Time-stamp: <Mon Jun 15 16:44:45 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/fileio/exr0401.c -
 *
 * Exercise 4.1: Implement tee using I/O system calls. By default, tee
 * overwrites any existing file with the given name. Implement the –a
 * command-line option (tee –a file), which causes tee to append text
 * to the end of a file if it already exists.
 * ========================================================================= */
#include "tlpi_hdr.h" 
#include <ctype.h>
#include <fcntl.h>

#define BSIZE 8
#define graphichar(ch) (isgraph((unsigned char)ch) ? ch : '#')

static void usageError(char *program, char *msg, int opt) {
  if (msg != NULL && opt != 0)
    fprintf(stderr, "%s (%c)\n", msg, graphichar(opt));
  fprintf(stderr, "Usage: %s [-a] filename\n", program);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[argc + 1]) {
  int opt, fd;
  ssize_t numRead;
  int flags;
  mode_t mode;
  char buf[BSIZE];

  mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  flags = O_WRONLY | O_CREAT | O_TRUNC;

  opterr = 0;
  while ((opt = getopt(argc, argv, "a")) != -1) {
    switch (opt) {
    case 'a':
      flags = O_WRONLY | O_CREAT | O_APPEND;
      break;
    case '?':
      usageError(argv[0], "Unrecognized option", optopt);
    default:
      usageError(argv[0], NULL, 0);
    }
  }
  /* Check missing required positional argument */
  if (optind >= argc)
    usageError(argv[0], NULL, 0);

  /* TODO: in fact, by default the real tee redirect to multiple
     argument files in provided beside standard output. */

  /* Read from stdin and write to: stdout and fd */
  if ((fd = open(argv[optind], flags, mode)) < 0)
    systmErr("open() failed");

  while ((numRead = read(STDIN_FILENO, buf, BSIZE)) > 0) {
    if (write(fd, buf, numRead) != numRead)
      systmErr("write() failed");
    if (write(STDOUT_FILENO, buf, numRead) != numRead)
      systmErr("write() failed");
  }
  if (numRead == -1)
    systmErr("read() failed");

  if (close(fd) < 0)
    systmErr("close() failed");

  return EXIT_SUCCESS;
}

/* Created on: Wed Jan 28 21:58:36 +01 2026 */
/* Decription: Sandbox for Listing 4-1 ./fileio/copy.c */
/* Purpose:    Demontrate basic I/O with syscall-wrapper functions
               open(2), read(2), write(2) and close(2). */
/* Universality of I/O

$ ./copy test.new test.old     # Copy a regular file
$ ./copy a.txt /dev/tty        # Copy a regular file to this terminal
$ ./copy /dev/tty b.txt        # Copy input from this terminal to regular file
$ ./copy /dev/pts/16 /dev/tty  # Copy input from another terminal
*/

#include "../lib/tlpi_hdr.h" // IWYU pragma: keep
#include <fcntl.h>
#include <sys/types.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

static inline int is_help(const char *s) {
  return (strcmp(s, "--help") == 0 || strcmp(s, "-h") == 0);
}

int main(int argc, char *argv[argc + 1]) {
  /* Declare stack variables */
  int srcFd, destFd, openFlags;
  mode_t mode;
  ssize_t numRead;
  char buf[BUF_SIZE];
  
  /* Argument validation */
  if (argc != 3 || is_help(argv[1]))
    usageErr("%s src-file dest-file\n", argv[0]);

  /* Open source file for read-only */
  srcFd = open(argv[1], O_RDONLY);
  if (srcFd == -1)
    systmErr("opening file '%s'", argv[1]);

  /* Open destination file for write */
  openFlags = O_WRONLY | O_CREAT | O_TRUNC;
  mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
  destFd = open(argv[2], openFlags, mode);
  if (destFd == -1)
    systmErr("opening file '%s'", argv[2]);

  /* Transfer data from source to destination */
  while ((numRead = read(srcFd, buf, BUF_SIZE)) > 0)
    if (write(destFd, buf, numRead) != numRead)
      custmErr("writing to destination file failed or partially completed");
  /* Check all read went to completion */
  if (numRead == -1)
    systmErr("reading from source file");

  /* Release file descriptors */
  if (close(srcFd) == -1)
    systmErr("close input");
  if (close(destFd) == -1)
    systmErr("close output");

  return EXIT_SUCCESS;
}

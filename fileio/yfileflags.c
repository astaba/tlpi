/* Created on: Sat Jan 31 13:50:51 +01 2026 */
/* Description: Section 5.3 Personal Practice */
/* Purpose: Using the fcntl(2) to get open file status flags. */
/*
$ ./yfileflags 0 </dev/tty
$ ./yfileflags 1 >tfile
$ cat tfile
$ ./yfileflags 2 2>tfile
$ ./yfileflags 5 5<>tfile
*/

#include "../lib/tlpi_hdr.h" // IWYU pragma: keep
#include <fcntl.h>

static inline int is_help(const char *h) {
  return (strcmp("--help", h) == 0) || (strcmp("-h", h) == 0);
}
int main(int argc, char *argv[]) {
  int fd, value;
  /* Validate arguments */
  if (argc < 2 || is_help(argv[1]))
    usageErr("%s <fileno>");
  /* String arg -> integer var */
  fd = getInt(argv[1], GN_NONNEG, argv[0]);

  value = fcntl(fd, F_GETFL);
  if (value == -1)
    systmErr("fcntl() failed");

  switch (value & O_ACCMODE) {
  case O_RDONLY:
    printf("read only, ");
    break;
  case O_WRONLY:
    printf("write only, ");
    break;
  case O_RDWR:
    printf("read and write, ");
    break;
  default:
    custmErr("Unknown access mode.");
  }

  if (value & O_APPEND)
    printf("append, ");
  if (value & O_NONBLOCK)
    printf("nonblocking, ");
  if (value & O_SYNC)
    printf("synchronous write.");

  putchar('\n');
  exit(0);
}

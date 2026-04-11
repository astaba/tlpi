/* =========================================================================
 * Created on: <Fri Apr 10 17:49:24 +01 2026>
 * Time-stamp: <Fri Apr 10 21:17:36 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/yread_line.c -
 *
 * What ever happen this routine returns a null-terminated string of
 * memoty bytes. Thus, it is enough to check for terminating '\n' to
 * ckeck for truncated data.
 * ========================================================================= */
#include "read_line.h" /* IWYU pragma: keep */
#include <errno.h>
#include <netinet/in.h>
#include <unistd.h>

ssize_t yreadLine(int fd, void *buffer, size_t n) {
  ssize_t numRead;  /* read() output */
  size_t readTally; /* # bytes read from fd into buffer */
  char ch;          /* temporary slot between read() and buffer */
  char *buf;        /* buffer modification tracker */

  /* Make buffer has at least 1 available slot */
  if (n <= 0 || buffer == NULL) {
    errno = EINVAL;
    return -1;
  }

  readTally = 0; /* Beware of stack garbage */
  buf = buffer;  /* cast void for pointer arithmetic */

  for (;;) {
    /* TODO: save syscalls with buffered I/O */
    numRead = read(fd, &ch, 1);

    if (numRead == -1) {
      if (errno == EINTR)
        continue; /* On intrrupt restart read() */
      else
        return -1;             /* return error */
    } else if (numRead == 0) { /* EOF hit */
      if (readTally == 0)
        return 0;
      else
        break;
    } else { /* read() successful */
      /* modify buffer only if one extra slot for '\0' */
      if (readTally + 1 < n) {
        *buf++ = ch;
        readTally++;
      }
      if (ch == '\n')
        break;
    }
  }

  *buf = '\0';
  return (ssize_t)readTally; /* To match read() output type */
}

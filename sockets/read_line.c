/* =========================================================================
 * Created on: <Fri Apr 10 16:58:24 +01 2026>
 * Time-stamp: <Mon Apr 13 03:00:50 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026
 *             See file [[file:../COPYING.gpl-v3]] for details GNU License.
 * Desc      : ~/coding/c_prog/tlpi/sockets/read_line.c -
 *
 * Listing 59.1 Implementation of readLine().  Read characters from
 * 'fd' until a newline is encountered.  If a newline character is not
 * encountered in the first (n - 1) bytes, then the excess characters
 * are discarded.  The returned string placed in 'buf' is
 * null-terminated and includes the newline character if it was read
 * in the first (n - 1) bytes.  The function return value is the
 * number of bytes placed in buffer (which includes the newline
 * character if encountered, but excludes the terminating null byte).
 * ========================================================================= */
#include "read_line.h" /* Declaration of readLine() */
#include <errno.h>
#include <unistd.h>

ssize_t readLine(int fd, void *buffer, size_t n) {
  ssize_t numRead; /* # of bytes fetched by last read() */
  size_t totRead;  /* Total bytes read so far */
  char *buf;
  char ch;

  if (n <= 0 || buffer == NULL) {
    errno = EINVAL;
    return -1;
  }

  buf = buffer; /* No pointer arithmetic on "void *" */

  totRead = 0;
  for (;;) {
    numRead = read(fd, &ch, 1);

    if (numRead == -1) {
      if (errno == EINTR) /* Interrupted --> restart read() */
        continue;
      else
        return -1; /* Some other error */

    } else if (numRead == 0) { /* EOF */
      if (totRead == 0)        /* No bytes read; return 0 */
        return 0;
      else /* Some bytes read; add '\0' */
        break;

    } else {                 /* 'numRead' must be 1 if we get here */
      if (totRead < n - 1) { /* Discard > (n - 1) bytes */
        totRead++;
        *buf++ = ch;
      }

      if (ch == '\n')
        break;
    }
  }

  *buf = '\0';
  return totRead;
}

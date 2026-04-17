/* =========================================================================
 * Created on: <Fri Apr 17 23:36:30 +01 2026>
 * Time-stamp: <Sat Apr 18 00:27:09 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/ybio.c -
 * Buffer I/O library
 * ========================================================================= */
#include "ybio.h"
#include <errno.h>
#include <string.h>
#include <unistd.h>

void ybio_init(int fd, ybio_t *p) {
  p->bio_fd = fd;
  p->bio_ptr = p->bio_buf;
  p->bio_cnt = 0;
}

static size_t ybio_read(ybio_t *p, void *buf, size_t n) {
  size_t cnt;

  while (p->bio_cnt <= 0) {
    p->bio_cnt = read(p->bio_fd, p->bio_buf, sizeof(p->bio_buf));
    if (p->bio_cnt < 0) {
      if (errno != EINTR)
        return -1; /* Return error condition */
    } else if (p->bio_cnt == 0)
      return 0; /* EOF */
    else
      p->bio_ptr = p->bio_buf;
  }

  cnt = n;
  if (p->bio_cnt < n)
    cnt = p->bio_cnt;

  memcpy((char *)buf, p->bio_ptr, cnt);
  p->bio_cnt -= cnt;
  p->bio_ptr += cnt;

  return cnt;
}

ssize_t ybio_readLine(ybio_t *p, void *buffer, size_t n) {
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
    numRead = read(p->bio_fd, &ch, 1);

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

/* =========================================================================
 * Created on: <Fri Apr 17 23:28:07 +01 2026> 
 * Time-stamp: <Sat Apr 18 11:27:16 +01 2026 by owner> 
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/ybio.h -
 * Buffered I/O library header file
 * ========================================================================= */

#ifndef YBIO_H
#define YBIO_H

#include <sys/types.h>

#define YBIO_SIZE 8192

typedef struct {
  int bio_fd;
  char *bio_ptr;
  char bio_buf[YBIO_SIZE];
  ssize_t bio_cnt;
} ybio_t;

void ybio_init(int fd, ybio_t *p);
ssize_t ybio_readl(ybio_t *p, void *buffer, size_t n);

#endif	/* YBIO_H */

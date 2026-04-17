/* =========================================================================
 * Created on: <Fri Apr 17 23:28:07 +01 2026> 
 * Time-stamp: <Sat Apr 18 00:34:39 +01 2026 by owner> 
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
  int bio_cnt;
} ybio_t;

#endif	/* YBIO_H */

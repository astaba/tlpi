/* =========================================================================
 * Created on: <Wed May 06 15:23:30 +01 2026>
 * Time-stamp: <Sat Jun 13 22:26:17 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/threads/exr3101_hdr.h -
 * ========================================================================= */
#ifndef EXR3101_HDR_H
#define EXR3101_HDR_H

#include <pthread.h>

typedef struct {
  bool once;
  pthread_mutex_t mutex;
} oncer_t;

#define ONCE_CONTROL_INITIALIZER                                               \
  { .once = false, .mutex = PTHREAD_MUTEX_INITIALIZER, }

int one_time_init(oncer_t *control, void (*init)(void));

#endif /* EXR3101_HDR_H */

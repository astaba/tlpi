/* =========================================================================
 * Created on: <Wed May 06 16:40:40 +01 2026>
 * Time-stamp: <Wed May  6 17:25:09 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/threads/exr3102.h -
 * ========================================================================= */
#ifndef EXR3102_HDR_H
#define EXR3102_HDR_H

#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include "pthread_wrappers.h"
#include <pthread.h>

char *ydirname(char *path);
char *ybasename(char *path);

#endif /* EXR3102_HDR_H */

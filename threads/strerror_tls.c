/* =========================================================================
 * Created on: <Tue May 05 15:48:30 +01 2026>
 * Time-stamp: <Tue May  5 15:52:19 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details
 * Desc      : ~/coding/c_prog/tlpi/threads/strerror_tls.c -
 *
 * Listing 31.4:
 * An implementation of strerror() that is made thread-safe through
 * the use of thread-local storage.
 * See also [[file:strerror_tsd.c]].
 * Thread-local storage requires: Linux 2.6 or later, NPTL, and gcc
 * 3.3 or later.
 * DEPRECATED:WONTFIX: sys_nerr and sys_errlist are removed from glibc
 * 2.32 on.  This code won't compile.
 * ========================================================================= */
/* Get '_sys_nerr' and '_sys_errlist' declarations from <stdio.h> */
#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <string.h> /* Get declaration of strerror() */

/* Maximum length of string in per-thread buffer returned by strerror() */
#define MAX_ERROR_LEN 256

static __thread char buf[MAX_ERROR_LEN];
/* Thread-local return buffer */

char *strerror(int err) {
  if (err < 0 || err >= _sys_nerr || _sys_errlist[err] == NULL) {
    snprintf(buf, MAX_ERROR_LEN, "Unknown error %d", err);
  } else {
    strncpy(buf, _sys_errlist[err], MAX_ERROR_LEN - 1);
    buf[MAX_ERROR_LEN - 1] = '\0'; /* Ensure null termination */
  }

  return buf;
}

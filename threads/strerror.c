/* =========================================================================
 * Created on: <Tue May 05 12:36:59 +01 2026>
 * Time-stamp: <Tue May  5 15:20:20 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details
 * Desc      : ~/coding/c_prog/tlpi/threads/strerror.c -
 *
 * Listing 31.1: An implementation of strerror() that is not thread-safe.
 * See [[file:strerror_test.c]]
 * DEPRECATED:WONTFIX: sys_nerr and sys_errlist are removed from glibc
 * 2.32 on.  This code won't compile.
 * ========================================================================= */
/* Get '_sys_nerr' and '_sys_errlist' declarations from <stdio.h> */
#define _GNU_SOURCE

#include <stdio.h>
#include <string.h> /* Get declaration of strerror() */

/* Maximum length of string returned by strerror() */
#define MAX_ERROR_LEN 256

static char buf[MAX_ERROR_LEN]; /* Statically allocated return buffer */

char *strerror(int err) {
  if (err < 0 || err >= _sys_nerr || _sys_errlist[err] == NULL) {
    snprintf(buf, MAX_ERROR_LEN, "Unknown error %d", err);
  } else {
    strncpy(buf, _sys_errlist[err], MAX_ERROR_LEN - 1);
    buf[MAX_ERROR_LEN - 1] = '\0'; /* Ensure null termination */
  }

  return buf;
}

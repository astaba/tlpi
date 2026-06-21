/* =========================================================================
 * Created on: <Fri Jun 05 18:35:03 +01 2026>
 * Time-stamp: <Fri Jun  5 19:00:22 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the files [[file:../COPYING.lgpl-v3]] and
 *             [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/time/curr_time.c -
 *
 * Listing 10.2: Implement our currTime() function.
 * ========================================================================= */
#include "curr_time.h" /* Declares function defined here */
#include <time.h>

#define BUF_SIZE 1000

/* Return a string containing the current time formatted according to
   the specification in 'format' (see strftime(3) for specifiers).
   If 'format' is NULL, we use "%c" as a specifier (which gives the'
   date and time as for ctime(3), but without the trailing newline).
   Returns NULL on error. */

char *currTime(const char *format) {
  static char buf[BUF_SIZE]; /* Nonreentrant */
  time_t t;
  size_t s;
  struct tm *tm;

  t = time(NULL);
  tm = localtime(&t);
  if (tm == NULL)
    return NULL;

  s = strftime(buf, BUF_SIZE, (format != NULL) ? format : "%c", tm);

  return (s == 0) ? NULL : buf;
}

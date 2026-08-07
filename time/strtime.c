/* =========================================================================
 * Created on: <Fri Jun 05 20:36:10 +01 2026>
 * Time-stamp: <Thu Jun 11 02:40:55 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/time/strtime.c -
 *
 * Listing 10.3: Demonstrate the use of strptime() and strftime().
 * Calls strptime() using the given "format" to process the
 * "input-date+time". The conversion is then reversed by calling
 * strftime() with the given "out-format" (or a default format if this
 * argument is omitted).
 *
 * Usage:
 * arg_1 - input-data-time: (required) user define date-time string.
 * arg_2 - input-format: (required) combination of
 * conversion-specifiers mapping arg_1 used by strptime() to correctly
 * interpret arg_1 on the model of scanf().
 * arg_3 - output-format: (optional) combination of
 * conversion-specifiers used by strftime() to output the conversion
 * of time_t output from strptime() back to a custom date-time
 * string. If undefined, a default format-string is used instead.
 * ========================================================================= */
#if !defined(__sun)
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif
#endif
#include "tlpi_hdr.h" /* IWYU pragma: keep */
#include <locale.h>
#include <time.h>

#define SBUF_SIZE 1000

int main(int argc, char *argv[]) {
  struct tm tm;
  char sbuf[SBUF_SIZE];
  char *ofmt;

  if (argc < 3 || strcmp(argv[1], "--help") == 0)
    usageErr("%s input-date-time in-format [out-format]\n", argv[0]);

  if (setlocale(LC_ALL, "") == NULL)
    systmErr("setlocale"); /* Use locale settings in conversions */

  memset(&tm, 0, sizeof(struct tm)); /* Initialize 'tm' */
  if (strptime(argv[1], argv[2], &tm) == NULL)
    custmErr("strptime");

  tm.tm_isdst = -1; /* Not set by strptime(); tells mktime()
                       to determine if DST is in effect */

  /* HACK: Calling mktime() serves double purposes:
   * 1. It returns the time_t seconds version of the input string;
   * 2. As you remember it modifies the input tm argument, and
   *    properly set up ignored (or cleared) fields before the tm
   *    struct is passed to strftime() */
  printf("calendar time (seconds since Epoch): %ld\n", (long)mktime(&tm));

  ofmt = (argc > 3) ? argv[3] : "%H:%M:%S %A, %d %B %Y %Z";
  if (strftime(sbuf, SBUF_SIZE, ofmt, &tm) == 0)
    custmErr("strftime returned 0");
  printf("strftime() yields: %s\n", sbuf);

  exit(EXIT_SUCCESS);
}

/* =========================================================================
 * Created on: <Fri Jun 05 17:57:42 +01 2026>
 * Time-stamp: <Sat Jun  6 14:38:45 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/time/calendar_time.c -
 *
 * Listing 10.1: Demonstrate the use of functions for working with
 * calendar time. This program retrieves the current time and displays
 * it in various forms.
 *
 * Usage: The TZ environment variable will affect localtime().
 * Try, for example:
 *
 *	$ ./calendar_time
 *	$ TZ=Pacific/Auckland ./calendar_time
 *
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <locale.h>
#include <sys/time.h>
#include <time.h>

#define SECONDS_IN_TROPICAL_YEAR (365.24219 * 24 * 60 * 60)

int main(int argc, char *argv[]) {
  time_t t;
  struct tm *gm_p, *loc_p;
  struct tm gm, loc;
  struct timeval tv;

  /* Retrieve time, convert and display it in various forms */

  t = time(NULL);
  printf("\nSeconds since the Epoch (1 Jan 1970): %ld", (long)t);
  printf(" (about %6.3f years)\n", t / SECONDS_IN_TROPICAL_YEAR);

  if (gettimeofday(&tv, NULL) == -1)
    systmErr("gettimeofday");
  printf("  gettimeofday() returned %ld secs, %ld microsecs\n", (long)tv.tv_sec,
         (long)tv.tv_usec);
  puts("");

  /* Convert the time_t to a UTC tm struct */

  gm_p = gmtime(&t);
  if (gm_p == NULL)
    systmErr("gmtime");
  /* Save local copy, since *gmp may be modified by asctime() or gmtime() */
  gm = *gm_p;

  /* Print the UTC tm struct fields */
  printf("Broken down by gmtime():\n");
  printf("  year=%d mon=%d mday=%d hour=%d min=%d sec=%d ", gm.tm_year,
         gm.tm_mon, gm.tm_mday, gm.tm_hour, gm.tm_min, gm.tm_sec);
  printf("wday=%d yday=%d isdst=%d\n", gm.tm_wday, gm.tm_yday, gm.tm_isdst);

  /* Convert time_t to a localized tm struct */

  loc_p = localtime(&t);
  if (loc_p == NULL)
    systmErr("localtime");
  loc = *loc_p; /* Save local copy */

  /* Print the localized tm struct fields */
  printf("Broken down by localtime():\n");
  printf("  year=%d mon=%d mday=%d hour=%d min=%d sec=%d ", loc.tm_year,
         loc.tm_mon, loc.tm_mday, loc.tm_hour, loc.tm_min, loc.tm_sec);
  printf("wday=%d yday=%d isdst=%d\n\n", loc.tm_wday, loc.tm_yday,
         loc.tm_isdst);

  /* Print the default time string from both a UTC and a localized output */
  printf("asctime() formats the gmtime() value as: %s", asctime(&gm));
  printf("ctime() formats the time() value as:     %s", ctime(&t));
  puts("");

  /* Convert back to time_t from both UTC tm and localized tm structs */
  printf("mktime() of gmtime() value:    %ld secs\n", (long)mktime(&gm));
  printf("mktime() of localtime() value: %ld secs\n", (long)mktime(&loc));

  exit(EXIT_SUCCESS);
}

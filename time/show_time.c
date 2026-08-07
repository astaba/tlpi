/* =========================================================================
 * Created on: <Sat Jun 06 13:13:58 +01 2026>
 * Time-stamp: <Sat Jun  6 13:21:49 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/time/show_time.c -
 *
 * Listing 10.4: A short program that allows us to see the effects of
 * locale and timezone on some of the functions that deal with time.
 *
 * Try running this program with command lines such as the following:
 *
 *      $ ./show_time
 *      $ TZ=":Pacific/Auckland" ./show_time
 *      $ TZ=":US/Central" ./show_time
 *      $ TZ=":CET" ./show_time
 *
 * ========================================================================= */
#include "tlpi_hdr.h" /* IWYU pragma: keep */
#include <locale.h>
#include <time.h>

#define BUF_SIZE 200

int main(int argc, char *argv[]) {
  time_t t;
  struct tm *loc;
  char buf[BUF_SIZE];

  if (setlocale(LC_ALL, "") == NULL)
    systmErr("setlocale"); /* Use locale settings in conversions */

  t = time(NULL);

  printf("ctime() of time() value is:  %s", ctime(&t));

  loc = localtime(&t);
  if (loc == NULL)
    systmErr("localtime");

  printf("asctime() of local time is:  %s", asctime(loc));

  if (strftime(buf, BUF_SIZE, "%A, %d %B %Y, %H:%M:%S %Z", loc) == 0)
    custmErr("strftime returned 0");
  printf("strftime() of local time is: %s\n", buf);

  exit(EXIT_SUCCESS);
}

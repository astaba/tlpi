/* =========================================================================
 * Created on: <Sat Jun 06 13:25:17 +01 2026>
 * Time-stamp: <Thu Jun 11 23:50:04 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/time/process_time.c -
 *
 * Listing 10.5: Demonstrate usage of clock(3) and times(2) to
 * retrieve process virtual times.
 *
 * Usage: process_time [num-calls]
 *
 * Make 'num-calls' calls to getppid(), and then display process times.
 *
 * TEST: While analysing output in the terminal try to diff
 * (substract) both arbitrary_uptime outputs, divide the diff by
 * clockTicks and compare to other results.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <sys/times.h>
#include <time.h>

static void /* Display 'msg' and process times */
displayProcessTimes(const char *msg) {
  /* To retrieve process group CPU times from times() */
  struct tms ts;
  /* HACK: System clock ticks: set it once during the process */
  static long clockTicks = 0;
  /* WARN: For historical reasons the clock_t types are different: */
  clock_t clockTime; /* clocks-per-sec returned by clock() */
  /* clock-ticks internally used and returned by times() */
  clock_t clockTickTime;

  if (msg != NULL)
    printf("%s", msg);

  /* HACK: Fetch clock ticks only on first call then keep value through
     out process life time courtesy of the 'static' keyword. */
  if (clockTicks == 0) {
    clockTicks = sysconf(_SC_CLK_TCK);
    if (clockTicks == -1)
      systmErr("sysconf");
  }

  clockTime = clock();
  if (clockTime == -1)
    systmErr("clock");

  printf("\tclock() returns: %ld clocks-per-sec (%.2f secs)\n", (long)clockTime,
         (double)clockTime / CLOCKS_PER_SEC);

  if ((clockTickTime = times(&ts)) == -1)
    systmErr("times");
  printf("\ttimes() yields: user CPU=%.2f; system CPU: %.2f\n",
         (double)ts.tms_utime / clockTicks, (double)ts.tms_stime / clockTicks);
  printf("\t\tArbitrary-Uptime: %ld clockticks\n", (long)clockTickTime);
}

int main(int argc, char *argv[]) {
  int numCalls, j;

  printf("CLOCKS_PER_SEC=%ld  sysconf(_SC_CLK_TCK)=%ld\n\n",
         (long)CLOCKS_PER_SEC, sysconf(_SC_CLK_TCK));

  displayProcessTimes("At program start:\n");

  /* Call getppid() a large number of times, so that
     some user and system CPU time are consumed */

  numCalls = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-calls") : 1000000;
  for (j = 0; j < numCalls; j++)
    (void)getppid();

  displayProcessTimes("After getppid() loop:\n");

  exit(EXIT_SUCCESS);
}

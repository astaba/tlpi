/* =========================================================================
 * Created on: <Thu May 14 09:47:27 +01 2026>
 * Time-stamp: <Sun May 17 19:06:56 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file COPYING.gpl-v3 for details.
 * Desc      : ~/coding/c_prog/tlpi/signals/intquit.c -
 *
 * Listing 20.2:
 * Catch the SIGINT and SIGQUIT signals, which are normally generated
 * by the control-C (^C) and control-\ (^\) keys respectively.
 * Note that although we use signal() to establish signal handlers in
 * this program, the use of sigaction() is always preferable for this
 * task.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h"
#include <signal.h>

static void sigHandler(int sig) {
  static int count = 0;

  /* WARN: This handler uses non-async-signal-safe functions
     (printf(), exit(); see Section 21.1.2) */

  if (sig == SIGINT) {
    count++;
    printf("Caught SIGINT (%d)\n", count); /* UNSAFE: */
    return; /* Resume execution at point of interruption */
  }

  /* Must be SIGQUIT - print a message and terminate the process */

  printf("Caught SIGQUIT - that's all folks!\n");
  exit(EXIT_SUCCESS);		/* UNSAFE: */
}

int main(int argc, char *argv[]) {
  /* Establish same handler for SIGINT and SIGQUIT. Here we use the
     simpler signal() API to establish a signal handler, but for the
     reasons described in Section 22.7 of TLPI, sigaction() is the
     (strongly) preferred API for this task. */

  if (signal(SIGINT, sigHandler) == SIG_ERR)
    systmErr("signal");
  if (signal(SIGQUIT, sigHandler) == SIG_ERR)
    systmErr("signal");

  for (;;)   /* Loop forever, waiting for signals */
    pause(); /* Block until a signal is caught */
}

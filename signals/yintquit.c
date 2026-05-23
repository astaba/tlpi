/* =========================================================================
 * Created on: <Thu May 14 10:32:03 +01 2026>
 * Time-stamp: <Sun May 17 19:06:56 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/signals/yintquit.c -
 *
 * Pratice [[file:intquit.c][Listing 20.2]]:
 * Catch the SIGINT and SIGQUIT signals, which are normally generated
 * by the control-C (^C) and control-\ (^\) keys respectively.
 * Note that although we use signal() to establish signal handlers in
 * this program, the use of sigaction() is always preferable for this
 * task.
 * ========================================================================= */
#define _GNU_SOURCE

#include "../lib/tlpi_hdr.h"
#include <signal.h>

static void handler(int sig) {
  static int count = 0;

  if (sig == SIGINT) {
    count++;
    /* UNSAFE: printf() is unsafe in async-signal handlers */
    printf("SIGINT caught: #[%d]\n", count);
    return; /* Resume execution at point of interruption. */
  }

  /* Must definitely be SIGQUIT */

  printf("SIGQUIT caught: Out of business!\n");
  /* UNSAFE: exit() is unsafe in async-signal handlers */
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[argc + 1]) {
  int j;

  if (signal(SIGINT, handler) == SIG_ERR)
    systmErr("signal() failed");
  if (signal(SIGQUIT, handler) == SIG_ERR)
    systmErr("signal() failed");

  for (j = 0;; j++) /* Loop forever waiting for signals */
    pause();        /* Block until a signal is caught */

  return EXIT_SUCCESS;
}

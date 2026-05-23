/* =========================================================================
 * Created on: <Thu May 14 20:22:57 +01 2026>
 * Time-stamp: <Fri May 15 10:05:09 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file COPYING.gpl-v3 for details.
 * Desc      : ~/coding/c_prog/tlpi/signals/sig_receiver.c -
 *
 * Listing 20.7
 *
 * Usage: sig_receiver [block-time]
 *
 * Catch and report statistics on signals sent by [[file:sig_sender.c]].
 *
 * Note that although we use signal() to establish the signal handler
 * in this program, the use of sigaction() is always preferable for
 * this task.
 * ========================================================================= */
#define _GNU_SOURCE
#include "../lib/tlpi_hdr.h"  /* IWYU pragma: keep */
#include "signal_functions.h" /* Declaration of printSigset() */
#include <signal.h>

static int sigCnt[NSIG]; /* Counts deliveries of each signal */
static volatile sig_atomic_t gotSigint = 0;
/* Set nonzero if SIGINT is delivered */

static void handler(int sig) {
  if (sig == SIGINT)
    gotSigint = 1;
  else
    sigCnt[sig]++;
}

int main(int argc, char *argv[]) {
  int n, numSecs;
  sigset_t pendingMask, blockingMask, emptyMask;

  printf("%s: PID is %ld\n", argv[0], (long)getpid());

  /* Here we use the simpler signal() API to establish a signal handler,
     but for the reasons described in Section 22.7 of TLPI, sigaction()
     is the (strongly) preferred API for this task. */

  for (n = 1; n < NSIG; n++)  /* Same handler for all signals */
    (void)signal(n, handler); /* Ignore errors */

  /* If a sleep time was specified, temporarily block all signals,
     sleep (while another process sends us signals), and then
     display the mask of pending signals and unblock all signals */

  if (argc > 1) {
    numSecs = getInt(argv[1], GN_GT_0, NULL);

    sigfillset(&blockingMask);
    if (sigprocmask(SIG_SETMASK, &blockingMask, NULL) == -1)
      systmErr("sigprocmask");

    printf("%s: sleeping for %d seconds\n", argv[0], numSecs);
    sleep(numSecs);

    if (sigpending(&pendingMask) == -1)
      systmErr("sigpending");

    printf("%s: pending signals are: \n", argv[0]);
    printSigset(stdout, "\t\t", &pendingMask);

    sigemptyset(&emptyMask); /* Unblock all signals */
    if (sigprocmask(SIG_SETMASK, &emptyMask, NULL) == -1)
      systmErr("sigprocmask");
  }

  while (!gotSigint) /* Loop until SIGINT caught */
    continue;

  for (n = 1; n < NSIG; n++) /* Display number of signals received */
    if (sigCnt[n] != 0)
      printf("%s: signal %d caught %d time%s\n", argv[0], n, sigCnt[n],
             (sigCnt[n] == 1) ? "" : "s");

  exit(EXIT_SUCCESS);
}

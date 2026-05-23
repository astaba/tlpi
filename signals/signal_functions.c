/* =========================================================================
 * Created on: <Thu May 14 17:01:59 +01 2026>
 * Time-stamp: <Sun May 17 20:56:21 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file COPYING.gpl-v3 for details.
 * Desc      : ~/coding/c_prog/tlpi/signals/signal_functions.c -
 *
 * Listing 20.4: Various useful functions for working with signals.
 * ========================================================================= */
#define _GNU_SOURCE
#include "signal_functions.h" /* Declares functions defined here */
#include "../lib/tlpi_hdr.h"
#include <signal.h>
#include <string.h>

/* WARNING:UNSAFE: All of the following functions employ fprintf(),
   which is not async-signal-safe (see Section 21.1.2). As such, these
   functions are also not async-signal-safe (i.e., beware of
   indiscriminately calling them from signal handlers). */

void /* Print list of signals within a signal set */
printSigset(FILE *of, const char *prefix, const sigset_t *sigset) {
  int sig, cnt;

  cnt = 0;
  for (sig = 1; sig < NSIG; sig++) {
    if (sigismember(sigset, sig)) {
      cnt++;
      fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
    }
  }

  if (cnt == 0)
    fprintf(of, "%s<empty signal set>\n", prefix);
}

int /* Print mask of blocked signals for this process */
printSigMask(FILE *of, const char *msg) {
  sigset_t currMask;

  if (msg != NULL)
    fprintf(of, "%s", msg);
  /* Retrieve current mask: when arg_2 is NULL arg_1 is ignored. */
  if (sigprocmask(SIG_BLOCK, NULL, &currMask) == -1)
    return -1;

  printSigset(of, "\t\t", &currMask);

  return 0;
}

int /* Print signals currently pending for this process */
printPendingSigs(FILE *of, const char *msg) {
  sigset_t pendingSigs;

  if (msg != NULL)
    fprintf(of, "%s", msg);

  if (sigpending(&pendingSigs) == -1)
    return -1;

  printSigset(of, "\t\t", &pendingSigs);

  return 0;
}

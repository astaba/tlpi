/* =========================================================================
 * Created on: <Sat May 23 12:20:54 +01 2026>
 * Time-stamp: <Sat May 23 14:04:12 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/signals/yabort.c -
 *
 * Exercise 21.1: Implement abort().
 * ========================================================================= */
#include "exr_signals.h"
#include <signal.h>
#include <stdio.h>

void yabort(void) {
  struct sigaction act;
  sigset_t mask;
  /* We need to raise SIGABRT to kill the process with core dump */

  /* Make sure SIGABRT is unblocked */
  sigemptyset(&mask);
  sigaddset(&mask, SIGABRT);
  sigprocmask(SIG_UNBLOCK, &mask, NULL);

  /* Iff SIGABRT is ignored reset default disposition */
  sigaction(SIGABRT, NULL, &act);
  if (act.sa_handler == SIG_IGN) {
    act.sa_handler = SIG_DFL;
    sigaction(SIGABRT, &act, NULL);
  }

  /* Then raise SIGABRT */
  printf("yabort: 1st raise()\n"); /* DEBUG */
  raise(SIGABRT);

  /* If we get here then the process caught SIGABRT with a custom
     handler and instead of _exit()'ing or siglongjmp() it returned */

  /* Unconditionally reset SIGABRT disposition to default */
  act.sa_handler = SIG_DFL;
  sigaction(SIGABRT, &act, NULL);

  /* And raise a final terminating SIGABRT */
  printf("yabort: 2nd raise()\n"); /* DEBUG */
  raise(SIGABRT);
}

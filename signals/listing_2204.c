/* =========================================================================
 * Created on: <Mon Jul 13 19:48:33 +01 2026>
 * Time-stamp: <Mon Jul 13 21:04:22 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/signals/listing_2204.c -
 *
 * Listing 22.4: Incorrectly unblocking and waiting for a signal
 * ========================================================================= */
#include "tlpi_hdr.h" 
#include <signal.h>

void handler(int sig) { /* Catch signal */ }

static void sandbox(void) {

  sigset_t prevMask, intMask;
  struct sigaction sa;

  sigemptyset(&intMask);
  sigaddset(&intMask, SIGINT);

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handler;

  if (sigaction(SIGINT, &sa, NULL) == -1)
    systmErr("sigaction");

  /* Block SIGINT prior to executing critical section. (At this point
   * we assume that SIGINT is not already blocked.) */

  if (sigprocmask(SIG_BLOCK, &intMask, &prevMask) == -1)
    systmErr("sigprocmask - SIG_BLOCK");

  /* Critical section: do some work here that must not be interrupted
   * by the SIGINT handler */

  /* End of critical section - restore old mask to unblock SIGINT */

  if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
    systmErr("sigprocmask - SIG_SETMASK");

  /* BUG: what if SIGINT arrives now... */

  pause(); /* Wait for SIGINT */
}

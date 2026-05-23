/* =========================================================================
 * Created on: <Thu May 14 19:21:47 +01 2026>
 * Time-stamp: <Thu May 14 19:46:35 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/signals/listing_2005.c -
 *
 * Listing 20.5: Temporarily blocking delivery of a signal.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h"
#include <signal.h>

static void sanbox(void) {

  sigset_t blockSet, prevMask;

  /* Initialize a signal set to contain SIGINT */
  sigemptyset(&blockSet);
  sigaddset(&blockSet, SIGINT);

  /* Block SIGINT and save previous signal mask */
  if (sigprocmask(SIG_BLOCK, &blockSet, &prevMask) == -1)
    systmErr("sigprocmask() failed");

  /* ... Code that should not be interrupted by SIGINT ... */

  /* Restore previous signal mask, unblocking SIGINT */
  if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
    systmErr("sigprocmask() failed");
}

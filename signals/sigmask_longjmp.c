/* =========================================================================
 * Created on: <Sun May 17 20:45:53 +01 2026>
 * Time-stamp: <Sat May 23 17:25:17 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/signals/sigmask_longjmp.c -
 *
 * Listing 21.2:
 * Demonstrate the different effects of longjmp() and siglongjmp() on
 * the process signal mask.
 * By default, this program uses setjmp() + longjmp().
 *
 * Compile with -DUSE_SIGSETJMP to use sigsetjmp() + siglongjmp().
 * make -k sigmask_longjmp
 * make -k sigmask_siglongjmp
 *
 * NOTICE: Using preprocessor derectives is the only way to alternate
 * both interfaces usage in both main and hanllder calls because SUSv3
 * doesn’t allow setjmp() and sigsetjmp() to be used within an
 * assignment statement (see Section 6.8).
 * ========================================================================= */
#define _GNU_SOURCE           /* Get strsignal() declaration from <string.h> */
#include "../lib/tlpi_hdr.h"  /* IWYU pragma: keep */
#include "signal_functions.h" /* Declaration of printSigMask() */
#include <setjmp.h>
#include <signal.h>
#include <string.h>

static volatile sig_atomic_t canJump = 0;
/* Set to 1 once "env" buffer has been
   initialized by [sig]setjmp() */
#ifdef USE_SIGSETJMP
static sigjmp_buf senv;
#else
static jmp_buf env;
#endif

static void handler(int sig) {
  /* UNSAFE: This handler uses non-async-signal-safe functions
     (printf(), strsignal(), printSigMask(); see Section 21.1.2) */

  printf("[SIG_HANDLER]: Received signal %d (%s), signal mask is:\n", sig,
         strsignal(sig));
  printSigMask(stdout, NULL);

  if (!canJump) {
    printf("'env' buffer not yet set, doing a simple return\n");
    return;
  }

#ifdef USE_SIGSETJMP
  siglongjmp(senv, 1);
#else
  longjmp(env, 1);
#endif
}

int main(int argc, char *argv[]) {
  struct sigaction sa;

  printSigMask(stdout, "[MAIN]: Signal mask at startup:\n");

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handler;
  if (sigaction(SIGINT, &sa, NULL) == -1)
    systmErr("sigaction");

#ifdef USE_SIGSETJMP
  printf("[MAIN]: Calling sigsetjmp()\n");
  if (sigsetjmp(senv, 1) == 0)
#else
  printf("[MAIN]: Calling setjmp()\n");
  if (setjmp(env) == 0)
#endif
    canJump = 1; /* Executed after [sig]setjmp() */

  else /* Executed after [sig]longjmp() */
    printSigMask(stdout, "[MAIN]: After jump from handler, signal mask is:\n");

  for (;;) /* Wait for signals until killed */
    pause();
}

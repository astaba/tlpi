/* =========================================================================
 * Created on: <Sat May 23 16:11:44 +01 2026>
 * Time-stamp: <Sat May 23 16:17:59 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See [[file:../COPYING.lgpl-v3]] and
 *             [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/signals/signal.c -
 *
 * Listing 22.1: An implementation of signal() using sigaction().
 *
 * Compiling with "-DOLD_SIGS" provides the older, unreliable signal
 * handler semantics (which are still the default on some System V
 * derivatives).
 *
 * Some UNIX implementations follow the BSD signal() semantics,
 * including Linux, Tru64, and of course the BSD derivatives. Others,
 * such as Solaris, follow the System V semantics. We'll conditionally
 * override signal() on platforms following the System V
 * semantics. For the other implementations, we'll provide a dummy
 * source file that doesn't implement signal().
 * ========================================================================= */
#if defined(__sun) || defined(__sgi)
#include <signal.h>

typedef void (*sighandler_t)(int);

sighandler_t signal(int sig, sighandler_t handler) {
  struct sigaction newDisp, prevDisp;

  newDisp.sa_handler = handler;
  sigemptyset(&newDisp.sa_mask);
#ifdef OLD_SIGNAL
  newDisp.sa_flags = SA_RESETHAND | SA_NODEFER;
#else
  newDisp.sa_flags = SA_RESTART;
#endif

  if (sigaction(sig, &newDisp, &prevDisp) == -1)
    return SIG_ERR;
  else
    return prevDisp.sa_handler;
}

#else
/* The following declaration is provided purely to avoid gcc's
   "warning: ISO C forbids an empty source file" warnings. */

extern int signalDummyVar;

#endif

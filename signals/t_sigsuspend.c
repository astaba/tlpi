/* =========================================================================
 * Created on: <Mon Jul 13 20:24:34 +01 2026>
 * Time-stamp: <Mon Jul 13 22:28:40 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/signals/t_sigsuspend.c -
 *
 * Listing 22.5: A short program to demonstrate why sigsuspend(&mask)
 * is preferable to calling sigprocmask(SIG_SETMASK, &mask, NULL) +
 * pause() separately. (By default this program uses sigsuspend(). To
 * make it use pause(), compile using "cc -DUSE_PAUSE".)
 *
 * Usage: t_sigsuspend [sleep-time]
 *
 * Send the SIGINT signal to this program by typing control-C (^C).
 * (Terminate the program using SIGQUIT, i.e., type control-\ (^\).)
 *
 * This program contains extra code that does not appear in the
 * version shown in the book. By defining USE_PAUSE when compiling, we
 * can replace the use of sigsuspend() by the nonatomic sigprocmask()
 * + pause(). This allows us to show that doing the latter way will
 * cause some signals to be lost.
 * ========================================================================= */
#include "signal_functions.h" /* Declarations of printSigMask()
                                           and printPendingSigs() */
#include "tlpi_hdr.h" /* IWYU pragma: keep */
#include <time.h>

/* Global variable incremented each time SIGINT is handled. NOTICE:
   Beside its usage doesn't matching its definition, as far as this
   experiment is concerned the variable is useless */
static volatile int sigintCnt = 0;

static volatile sig_atomic_t gotSigquit = 0;

static void handler(int sig) {
  printf("Caught signal %d (%s)\n", sig, strsignal(sig));
  /* UNSAFE (see Section 21.1.2) */
  if (sig == SIGQUIT)
    gotSigquit = 1;
  sigintCnt++;
}

int main(int argc, char *argv[]) {
  int loopNum;
#ifdef USE_PAUSE
  int sleepTime;
#endif
  time_t startTime;
  sigset_t origMask, blockMask;
  struct sigaction sa;

  /* (1) */
  printSigMask(stdout, "Initial signal mask is:\n");

  sigemptyset(&blockMask);
  sigaddset(&blockMask, SIGINT);
  sigaddset(&blockMask, SIGQUIT);
#ifdef USE_PAUSE
  sleepTime = (argc > 1) ? getInt(argv[1], GN_NONNEG, NULL) : 0;
#endif

  /* (2) Block SIGINT and SIGQUIT - at this point we assume that these
   *  signals are not already blocked (obviously true in this simple
   *  program) so that 'origMask' will not contain either of these
   *  signals after the call. */

  if (sigprocmask(SIG_BLOCK, &blockMask, &origMask) == -1)
    systmErr("sigprocmask - SIG_BLOCK");

  /* (3) Set up handlers for SIGINT and SIGQUIT */

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handler;
  if (sigaction(SIGINT, &sa, NULL) == -1)
    systmErr("sigaction");
  if (sigaction(SIGQUIT, &sa, NULL) == -1)
    systmErr("sigaction");

  /* (4) Loop until SIGQUIT received */

  for (loopNum = 1; !gotSigquit; loopNum++) {
    printf("=== LOOP %d\n", loopNum);

    printSigMask(stdout, "Starting critical section, signal mask is:\n");
    /* Simulate a critical section by delaying a few seconds */
    for (startTime = time(NULL); time(NULL) < startTime + 4;)
      continue; /* Run for a few seconds elapsed time */

#ifndef USE_PAUSE
    /* The right way: use sigsuspend() to atomically unblock
       signals and pause waiting for signal */
    printPendingSigs(stdout, "Before sigsuspend() - pending signals:\n");

    if (sigsuspend(&origMask) == -1 && errno != EINTR)
      systmErr("sigsuspend");
#else

    /* The wrong way: unblock signal using sigprocmask(),
       then pause() */

    if (sigprocmask(SIG_SETMASK, &origMask, NULL) == -1)
      systmErr("sigprocmask - SIG_SETMASK");

    /* At this point, if SIGINT arrives, it will be caught and
       handled before the pause() call and, in consequence,
       pause() will block. (And thus only another SIGINT signal
       AFTER the pause call() will actually cause the pause()
       call to be interrupted.)  Here we make the window between
       the two calls a bit larger so that we have a better
       chance of sending the signal. */

    if (sleepTime > 0) {
      printf("Unblocked SIGINT, now waiting for %d seconds\n", sleepTime);
      for (startTime = time(NULL); time(NULL) < startTime + sleepTime;)
        continue;
      printf("Finished waiting - now going to pause()\n");
    }

    /* And now wait for the signal */

    pause();

    printf("Signal count = %d\n", sigintCnt);
    sigintCnt = 0;
#endif
  }

  /* (5) Restore signal mask so that signals are unblocked */

  if (sigprocmask(SIG_SETMASK, &origMask, NULL) == -1)
    systmErr("sigprocmask - SIG_SETMASK");

  /* (6) */
  printSigMask(stdout, "=== Exited loop\nRestored signal mask to:\n");

  /* Do other processing... */

  exit(EXIT_SUCCESS);
}

/* =========================================================================
 * Created on: <Fri May 15 19:27:52 +01 2026>
 * Time-stamp: <Sat May 23 13:13:51 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/signals/exr2001.c -
 *
 * Exercise 20.1: As noted in Section 20.3, sigaction() is more
 * portable than signal() for establishing a signal handler. Replace
 * the use of signal() by sigaction() in the program in Listing 20.7
 * [[file:sig_receiver.c]].
 * ========================================================================= */
#define _GNU_SOURCE
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <signal.h>
#include <stdbool.h>

static int sigCnt[NSIG];
static volatile sig_atomic_t gotSigint = false;

static void handler(int sig) {
  if (sig == SIGINT) {
    gotSigint = true;
    return;
  }
  sigCnt[sig]++;
}

int main(int argc, char *argv[argc + 1]) {
  struct sigaction sa;
  sigset_t blockMask, pendingMask, unblockMask;
  pid_t pid;
  int n, numSecd, cnt;

  if (argc > 1 && strcmp(argv[1], "--help") == 0) {
    fprintf(stdout, "Usage: %s [block-time]\n", argv[0]);
    exit(EXIT_SUCCESS);
  }
  pid = getpid();
  printf("[PID=%ld]%s\n", (long)pid, argv[0]);

  /* ANSWER: Register signal handler using sigaction() */
  sa.sa_handler = handler;
  (void)sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;             /* Leave system implementation default flags */
  for (n = 1; n < NSIG; n++) { /* Register same handler for all signals */
    if (n == SIGSTOP || n == SIGKILL) /* Skip kernel-destinated signals */
      continue;
    if (sigaction(n, &sa, NULL) == -1)
      /* Some systems return errors for unused signal numbers, so we can check
       * and continue or fail gracefully */
      if (errno != EINVAL)
        systmErr("sigaction() failed");
  }

  /* If argv[1] then block all signals, sleep(argv[1]), display
     pending signals and unblock all signals. */
  if (argc == 2) {
    numSecd = getInt(argv[1], GN_GT_0, "num-secd");

    sigfillset(&blockMask);
    if (sigprocmask(SIG_SETMASK, &blockMask, NULL) == -1)
      systmErr("sigprocmask() failed");

    printf("[PID=%ld]%s: sleep for %d seconds\n", (long)pid, argv[0], numSecd);
    sleep(numSecd);

    if (sigpending(&pendingMask) == -1)
      systmErr("sigpending() failed");

    printf("[PID=%ld]%s: pending signals are:\n", (long)pid, argv[0]);
    cnt = 0;
    for (n = 1; n < NSIG; n++) {
      if (sigismember(&pendingMask, n)) {
        printf("\t\tSig n°%d (%s)\n", n, strsignal(n));
        cnt++;
      }
    }
    if (cnt == 0)
      printf("\t\t<empty pending mask>\n");

    sigemptyset(&unblockMask);
    if (sigprocmask(SIG_SETMASK, &unblockMask, NULL) == -1)
      systmErr("sigprocmask() failed");
  }

  /* Wait indefinitely for arrival of signal SIGINT */
  while (gotSigint == false)
    pause();
  printf("[PID=%ld]%s: caught sig n°%d: break out of pause-loop\n", (long)pid,
         argv[0], SIGINT);

  /* Check all signals caught */
  for (n = 1; n < NSIG; n++)
    if (sigCnt[n] > 0)
      printf("[PID=%ld]%s: caught sig n°%d %d time%s\n", (long)pid, argv[0], n,
             sigCnt[n], (sigCnt[n] < 2) ? "" : "s");

  return EXIT_SUCCESS;
}

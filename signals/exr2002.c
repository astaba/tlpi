/* =========================================================================
 * Created on: <Sat May 16 12:31:52 +01 2026>
 * Time-stamp: <Sat May 23 00:43:47 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/signals/exr2002.c -
 *
 * Exercise 20.2: Write a program that shows that when the disposition
 * of a pending signal is changed to be SIG_IGN, the program never
 * sees (catches) the signal.
 * ========================================================================= */
#define _GNU_SOURCE
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <signal.h>

static void handler(int sig) {
  printf("[SIGINT_HANDLER]: Caught sig n°%d\n", sig); /* UNSAFE: */
  return;
}

int main(int argc, char *argv[argc + 1]) {
  struct sigaction sa;
  sigset_t blockMask, pendingMask, prevMask;
  pid_t pid;

  pid = getpid();

  /* 1. Register SIG handler */
  (void)sigemptyset(&sa.sa_mask);
  sa.sa_handler = handler;
  sa.sa_flags = 0;
  if (sigaction(SIGINT, &sa, NULL) == -1)
    systmErr("sigaction() failed");

  /* 2. Raise SIG to test handler */
  printf("[%ld]%s: Raising SIGINT to test handler\n", (long)pid, argv[0]);
  if (raise(SIGINT) != 0)
    systmErr("raise() failed");

  /* 3. Block SIG */
  (void)sigemptyset(&blockMask);
  (void)sigaddset(&blockMask, SIGINT);
  if (sigprocmask(SIG_BLOCK, &blockMask, &prevMask) == -1)
    systmErr("sigprocmask() failed");

  /* 4. Raised SIG while blocked to turn on its pending vector bit */
  printf("[%ld]%s: Raising SIGINT currently blocked\n", (long)pid, argv[0]);
  if (raise(SIGINT) != 0)
    systmErr("raise() failed");

  /* 5. Check SIG pending vector bit is on */
  if (sigpending(&pendingMask) == -1)
    systmErr("sigpending() failed");
  if (sigismember(&pendingMask, SIGINT))
    printf("[%ld]%s: SIGINT is confirmed PENDING\n", (long)pid, argv[0]);
  else
    custmErr("Failure in the pending bit vector!\n");

  /* 6. ANSWER: While blocked, change SIG disposition to SIG_IGN */
  printf("[%ld]%s: Changing SIGINT disposition to SIG_IGN\n", (long)pid,
         argv[0]);
  sa.sa_handler = SIG_IGN;
  if (sigaction(SIGINT, &sa, NULL) == -1)
    systmErr("sigaction() failed");

  /* 7. Unblock SIG */
  printf("[%ld]%s: Unblockig SIGINT...\n", (long)pid, argv[0]);
  if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
    systmErr("sigprocmask() failed");

  /* 8. Double check pending bit vector */
  if (sigpending(&pendingMask) == -1)
    systmErr("sigpending() failed");
  if (sigismember(&pendingMask, SIGINT))
    printf("[%ld]%s: FAILED. SIGINT is somehow still pending\n", (long)pid,
           argv[0]);
  else
    printf("[%ld]%s: If handler didn't trigger a second time\n"
           "\tthen SIGINT successfully vanished from pending bit vector\n",
           (long)pid, argv[0]);

  return EXIT_SUCCESS;
}

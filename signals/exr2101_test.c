/* =========================================================================
 * Created on: <Sat May 23 12:55:00 +01 2026>
 * Time-stamp: <Sat May 23 14:00:02 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/signals/exr2101_test.c -
 *
 * Test Exercise 21.1 [[file:yabort.c]]
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include "exr_signals.h"
#include <setjmp.h>
#include <signal.h>

sigjmp_buf env;

static void handlerjmp(int sig) {
  printf("[handlerjmp]: Nonlocal jump from SIGABRT handler\n");
  siglongjmp(env, 1);
}

static void handler(int sig) {
  printf("[handler]: Returning from SIGABRT handler\n");
}

int main(int argc, char *argv[argc + 1]) {
  struct sigaction act;
  sigset_t mask;
  pid_t pid = getpid();
  int testno;

  testno = (argc > 1) ? getInt(argv[1], GN_GT_0, "test-no") : 1;

  switch (testno) {
  case 1:
    /* 1. Block SIGABRT */
    printf("[%ld]main: Blocking SIGABRT\n", (long)pid);
    (void)sigemptyset(&mask);
    (void)sigaddset(&mask, SIGABRT);
    if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
      systmErr("sigprocmask() failed");
    break;
  case 2:
    /* 2. Ignore SIGABRT */
    printf("[%ld]main: Ignoring SIGABRT\n", (long)pid);
    act.sa_handler = SIG_IGN;
    (void)sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (sigaction(SIGABRT, &act, NULL) == -1)
      systmErr("sigaction() failed");
    break;
  case 3:
    /* 3. Set handler that returns */
    printf("[%ld]main: Set returning handler for SIGABRT\n", (long)pid);
    act.sa_handler = handler;
    (void)sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if (sigaction(SIGABRT, &act, NULL) == -1)
      systmErr("sigaction() failed");
    break;
  case 4:
    /* 4. Set handler that doesn't return */
    printf("[%ld]main: Set jumping handler for SIGABRT\n", (long)pid);
    if (sigsetjmp(env, 1) == 0) {
      act.sa_handler = handlerjmp;
      (void)sigemptyset(&act.sa_mask);
      act.sa_flags = 0;
      if (sigaction(SIGABRT, &act, NULL) == -1)
        systmErr("sigaction() failed");
    } else {
      printf("[%ld]main: Bypassed SIGABRT to exit() gracefully\n", (long)pid);
      return EXIT_SUCCESS;
    }
    break;
  default:
    usageErr("Test number not supported: %d\n", testno);
  }

  printf("[%ld]main: Calling yabort()\n", (long)pid);
  yabort();

  printf("[%ld]main: TEST FAILURE Flow control should never get this far!\n",
         (long)pid);
  return EXIT_SUCCESS;
}

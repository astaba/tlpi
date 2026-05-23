/* =========================================================================
 * Created on: <Sat May 16 20:23:26 +01 2026>
 * Time-stamp: <Sun May 17 19:06:56 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/signals/exr2003_resethand.c -
 *
 * Exercise 20.3: Write programs that verify the effect of the
 * SA_RESETHAND and SA_NODEFER flags when establishing a signal
 * handler with sigaction().
 * See also [[file:exr2003_nodefer.c]]
 * ========================================================================= */
#define _GNU_SOURCE
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <signal.h>

static void handler(int sig) {
  printf("[SIGINT_HANDLER]: Caught sig n°%d. The fire next time\n",
         sig); /* UNSAFE: */
  return;
}

int main(int argc, char *argv[argc + 1]) {
  struct sigaction sa;
  pid_t pid;

  pid = getpid();

  /* 1. Register SIG handler */
  (void)sigemptyset(&sa.sa_mask);
  sa.sa_handler = handler;
  sa.sa_flags = SA_RESETHAND;
  if (sigaction(SIGINT, &sa, NULL) == -1)
    systmErr("sigaction() failed");

  /* 2. Raise SIG to test handler */
  printf("[%ld]%s: Raising SIGINT to test handler\n", (long)pid, argv[0]);
  if (raise(SIGINT) != 0)
    systmErr("raise() failed");

  /* 3. Raise SIG again to trigger the effect of SA_RESETHAND flag */
  printf("[%ld]%s: Raising SIGINT to trigger SA_RESETHAND flag effect\n",
         (long)pid, argv[0]);
  if (raise(SIGINT) != 0)
    systmErr("raise() failed");

  /* 4. Test statement */
  printf("[%ld]%s: Unless test failure this should never display\n", (long)pid,
         argv[0]);

  return EXIT_SUCCESS;
}

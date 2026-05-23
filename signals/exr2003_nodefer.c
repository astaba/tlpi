/* =========================================================================
 * Created on: <Sat May 16 20:25:35 +01 2026>
 * Time-stamp: <Sun May 17 19:06:56 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/signals/exr2003_nodefer.c -
 *
 * Exercise 20.3: Write programs that verify the effect of the
 * SA_RESETHAND and SA_NODEFER flags when establishing a signal
 * handler with sigaction().
 * See also [[file:exr2003_resethand.c]]
 * ========================================================================= */
#define _GNU_SOURCE
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <signal.h>

static volatile sig_atomic_t depth = 0;

static void handler(int sig) {
  /* UNSAFE: All printf() and exit() calls */
  int current_depth = depth;
  current_depth++;
  depth = current_depth;
  printf("[SIG(%d)_HANDLER:DEPTH(%d)]: Entering handler\n", sig, depth);

  if (current_depth == 1) {
    printf("[SIG(%d)_HANDLER:DEPTH(%d)]: Raise recursive signal to test "
           "SA_NODEFER flag\n",
           sig, depth);
    /* Raise nested signal to call handler recursively if SA_NODEFER is set */
    if (raise(SIGINT) != 0)
      systmErr("raise() failed");

    printf("[SIG(%d)_HANDLER:DEPTH(%d)]: Resuming after raising recursive "
           "signal and exit process\n",
           sig, depth);
    exit(EXIT_SUCCESS);
  }
  printf("[SIG(%d)_HANDLER:DEPTH(%d)]: Exit recursive handler call\n", sig,
         depth);

  current_depth--;
  depth = current_depth;
}

int main(int argc, char *argv[argc + 1]) {
  struct sigaction sa;
  pid_t pid;
  int toggle;

  if (argc > 1 && !strcmp(argv[1], "--help"))
    usageErr("%s [1 or 0 flag to toggle SA_NODEFER]\n", argv[0]);

  pid = getpid();
  toggle = (argc > 1) ? getInt(argv[1], GN_NONNEG, "toggle") : 1;

  /* 1. Register SIG handler */
  sa.sa_handler = handler;
  (void)sigemptyset(&sa.sa_mask);
  sa.sa_flags = (toggle) ? SA_NODEFER : 0;
  if (sigaction(SIGINT, &sa, NULL) == -1)
    systmErr("sigaction() failed");

  /* 2. Raise SIG to test handler */
  printf("[%ld]%s: Raising initial SIGINT\n", (long)pid, argv[0]);
  if (raise(SIGINT) != 0)
    systmErr("raise() failed");

  printf("[%ld]%s: Program finish cleanly\n", (long)pid, argv[0]);
  return EXIT_SUCCESS;
}

/* =========================================================================
 * Created on: <Sat May 16 22:14:13 +01 2026>
 * Time-stamp: <Sun May 17 11:44:28 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/signals/exr2004.c -
 *
 * Exrcise 20.4: Implement siginterrupt() using sigaction().
 *
 * NOTICE: SIGINT is preferred because is the easiest to send from the
 * keyboard. However, this should work with any other signal
 * (user-oriented)
 * ========================================================================= */
#define _GNU_SOURCE
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <signal.h>

#define BSIZE 128

static void handler(int sig) { return; }

/* Mock the now DEPRECATED siginterrupt(). Prefix with "y_" to avoid
 * namespace confusion. */
static int y_siginterrupt(int sig, int flag) {
  struct sigaction sa;

  /* 1. Retreive current sigaction struct */
  if (sigaction(sig, NULL, &sa) == -1)
    return -1;

  if (flag) /* 2. Disable restart */
    sa.sa_flags &= ~SA_RESTART;
  else /* 3. Confirm the default which enable restart */
    sa.sa_flags |= SA_RESTART;

  /* 4. Set the new sigaction struct */
  if (sigaction(sig, &sa, NULL) == -1)
    return -1;

  return 0;
}

int main(int argc, char *argv[argc + 1]) {
  struct sigaction sa;
  char buf[BSIZE];
  ssize_t numRead;
  int toggle; /* Set flag from the command line */

  if (argc > 1 && !strcmp(argv[1], "--help"))
    usageErr("%s [1 or 0 for siginterrupt boolean flag]\n", argv[0]);

  toggle = (argc > 1) ? getInt(argv[1], GN_NONNEG, "toggle") : 1;

  /* Register sig handler using sigaction() */
  sa.sa_handler = handler;
  (void)sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGINT, &sa, NULL) == -1)
    systmErr("sigaction() failed");

  /* Reset system call response to signal interruption */
  if (y_siginterrupt(SIGINT, toggle) == -1)
    systmErr("y_siginterrupt() failed");

  /* NOTICE: Launch system call to test SA_RESTART flag. While read()
   * system call is blocking waiting for input whether with partial
   * input or nothing at all, trigger the signal. */
  numRead = read(STDIN_FILENO, buf, BSIZE);
  if (numRead == -1) {
    if (errno == EINTR)
      printf("read() failed with EINTR (syscall successfully interrupted)\n");
    else
      systmErr("read() failed");
  } else {
    buf[numRead] = '\0';
    /* buf[strcspn(buf, "\n")] = '\0'; */
    printf("Input: %s", buf);
  }

  return EXIT_SUCCESS;
}

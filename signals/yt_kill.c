/* =========================================================================
 * Created on: <Thu May 14 15:43:01 +01 2026>
 * Time-stamp: <Thu May 14 16:05:18 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/signals/yt_kill.c -
 *
 * Practice [[file:t_kill.c][Listing 20.3 t_kill]]:
 * Using the kill() system call
 * ========================================================================= */
#include "../lib/tlpi_hdr.h"	/* IWYU pragma: keep */
#include <signal.h>

int main(int argc, char *argv[argc + 1]) {
  int sig, rc;

  if (argc < 3 || !strcmp(argv[1], "--help"))
    usageErr("%s pid sig-num\n", argv[0]);

  sig = getInt(argv[2], GN_NONNEG, "sig-num");

  rc = kill(getLong(argv[1], 0, "pid"), sig);
  if (sig != 0) { /* Send signal */
    if (rc == -1)
      systmErr("kill() failed");
  } else { /* Check process existence */
    if (rc == 0)
      printf("Process exists and we can sent signals\n");
    else {
      if (errno == EPERM)
        printf("Process exists but we have no permission to send it signals\n");
      else if (errno == ESRCH)
        printf("Process does not exist\n");
      else
        systmErr("kill() failed");
    }
  }

  return EXIT_SUCCESS;
}

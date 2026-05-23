/* =========================================================================
 * Created on: <Thu May 14 15:38:30 +01 2026>
 * Time-stamp: <Thu May 14 16:04:15 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file COPYING.gpl-v3 for details.
 * Desc      : ~/coding/c_prog/tlpi/signals/t_kill.c -
 *
 * Listing 20.3:
 * Send a signal using kill(2) and analyze the return status of the
 * call.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h"	/* IWYU pragma: keep */
#include <signal.h>

int main(int argc, char *argv[]) {
  int s, sig;

  if (argc != 3 || strcmp(argv[1], "--help") == 0)
    usageErr("%s pid sig-num\n", argv[0]);

  sig = getInt(argv[2], 0, "sig-num");

  s = kill(getLong(argv[1], 0, "pid"), sig);

  if (sig != 0) {
    if (s == -1)
      systmErr("kill");

  } else { /* Null signal: process existence check */
    if (s == 0) {
      printf("Process exists and we can send it a signal\n");
    } else {
      if (errno == EPERM)
        printf("Process exists, but we don't have "
               "permission to send it a signal\n");
      else if (errno == ESRCH)
        printf("Process does not exist\n");
      else
        systmErr("kill");
    }
  }

  exit(EXIT_SUCCESS);
}

/* =========================================================================
 * Created on: <Thu May 14 20:20:08 +01 2026>
 * Time-stamp: <Thu May 14 20:28:45 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file COPYING.gpl-v3 for details.
 * Desc      : ~/coding/c_prog/tlpi/signals/sig_sender.c -
 *
 * Listing 20.6
 *
 * Usage: sig_sender PID num-sigs sig [sig2]
 *
 * Send signals to [[file:sig_receiver.c]].
 *
 * Sends 'num-sigs' signals of type 'sig' to the process with the
 * specified PID. If a fourth command-line argument is supplied, send
 * one instance of that signal, after sending the previous signals.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h"	/* IWYU pragma: keep */
#include <signal.h>

int main(int argc, char *argv[]) {
  int numSigs, sig, j;
  pid_t pid;

  if (argc < 4 || strcmp(argv[1], "--help") == 0)
    usageErr("%s pid num-sigs sig-num [sig-num-2]\n", argv[0]);

  pid = getLong(argv[1], 0, "PID");
  numSigs = getInt(argv[2], GN_GT_0, "num-sigs");
  sig = getInt(argv[3], 0, "sig-num");

  /* Send signals to receiver */

  printf("%s: sending signal %d to process %ld %d times\n", argv[0], sig,
         (long)pid, numSigs);

  for (j = 0; j < numSigs; j++)
    if (kill(pid, sig) == -1)
      systmErr("kill");

  /* If a fourth command-line argument was specified, send that signal */

  if (argc > 4)
    if (kill(pid, getInt(argv[4], 0, "sig-num-2")) == -1)
      systmErr("kill");

  printf("%s: exiting\n", argv[0]);
  exit(EXIT_SUCCESS);
}

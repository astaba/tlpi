/* =========================================================================
 * Created on: <Fri May 15 08:19:46 +01 2026>
 * Time-stamp: <Fri May 15 10:28:26 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/signals/ysig_sender.c -
 *
 * Listing 20.6
 *
 * Usage: ysig_sender PID num-sigs sig [sig2]
 *
 * Send signals to [[file:sig_receiver.c]].
 *
 * Sends 'num-sigs' signals of type 'sig' to the process with the
 * specified PID. If a fourth command-line argument is supplied, send
 * one instance of that signal, after sending the previous signals.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <signal.h>

int main(int argc, char *argv[argc + 1]) {
  long pid;
  int numSigs, sig, j;

  /* Arguments validation */
  if (argc < 4 || strcmp(argv[1], "--help") == 0 || argc > 5)
    usageErr("%s PID num-sigs sig [sig2]\n", argv[0]);

  /* Retrieve required arguments */
  pid = getLong(argv[1], 0, "PID");
  numSigs = getInt(argv[2], GN_GT_0, "num-sigs");
  sig = getInt(argv[3], 0, "sig");

  /* Notify and send signals */
  printf("%s: send signal n°%d %d times to process %ld\n", argv[0], sig, numSigs,
         pid);
  for (j = 0; j < numSigs; j++)
    if (kill(pid, sig) == -1)
      systmErr("kill() failed");

  /* If second-signal argument, send it too */
  if (argc == 5)
    if (kill(pid, getInt(argv[4], 0, "sig2")) == -1)
      systmErr("kill() failed");

  /* Notify exit */
  printf("%s: exiting\n", argv[0]);
  return EXIT_SUCCESS;
}

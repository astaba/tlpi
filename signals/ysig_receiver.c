/* =========================================================================
 * Created on: <Fri May 15 08:43:21 +01 2026>
 * Time-stamp: <Mon May 18 10:15:08 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/signals/ysig_receiver.c -
 *
 * Listing 20.7
 *
 * Usage: ysig_receiver [block-time]
 *
 * Catch and report statistics on signals sent by [[file:sig_sender.c]].
 * See [[file:sig_receiver.c]]
 * Note that although we use signal() to establish the signal handler
 * in this program, the use of sigaction() is always preferable for
 * this task.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h"
#include "signal_functions.h"
#include <signal.h>
#include <stdbool.h>

/* Each slot index is the numeric value of the slot-resident
   signal. Since the smallest signal number is 1 and NSIG is the upper
   bound of the highest signal, this array is expected to be sparsed
   form the start, i.e. slot[0] is unused (beside slots matching
   SIGKILL and SIGSTOP values). WARNING: since each slot is used for
   counting, this design relies entirely on the compiler zeroing out
   all static variable in BSS segment, the default location of
   uninitialized static variables. */
static int sigCnt[NSIG];
static volatile sig_atomic_t gotSigint = false;

/* Keep tally of the number of times each signal was caught and
   handled, except SIGINT which disposition is changed to merely set a
   static flag needed by the main routine to break out of the infinite
   loop consuming CPU time. */
static void sighandler(int sig) {
  if (sig == SIGINT) {
    gotSigint = true;
    return;
  }
  sigCnt[sig]++;
}

int main(int argc, char *argv[argc + 1]) {
  sigset_t blockSet, pendingSet, emptySet;
  int j, st, cnt;

  /* Display PID for the sender to use in terminal session */
  printf("%s: process ID %ld\n", argv[0], (long)getpid());

  /* Register same handler for all user-manageable signals. NOTICE: we
     must ignore signal() errors on SIGKILL and SIGSTOP which are sent
     to the Kernel to take action on user. */
  for (j = 1; j < NSIG; j++)
    signal(j, sighandler);

  /* If argv[1] then block all signals, sleep(argv[1]), display
     pending signal(s) and unblock all signals. */
  if (argc == 2) {
    sigfillset(&blockSet);
    if (sigprocmask(SIG_SETMASK, &blockSet, NULL) == -1)
      systmErr("sigprocmask() failed\n");

    st = getInt(argv[1], GN_GT_0, "sleep-time");
    printf("%s: going to sleep\n", argv[0]);
    sleep(st);

    if (sigpending(&pendingSet) == -1)
      systmErr("sigpending() failed");

    printf("%s: pending signals are:\n", argv[0]);
    printSigset(stdout, "\t\t", &pendingSet);

    sigemptyset(&emptySet);
    if (sigprocmask(SIG_SETMASK, &emptySet, NULL) == -1)
      systmErr("sigprocmask() failed\n");
  }

  /* Loop infinitely to consume CPU time until SIGINT is caught. */
  while (gotSigint == false)
    ;

  for (j = 1; j < NSIG; j++) {
    cnt = sigCnt[j];
    if (cnt)
      printf("%s: caught signal n°%d %d time%s\n", argv[0], j, cnt,
             (cnt > 1) ? "s" : "");
  }

  return EXIT_SUCCESS;
}

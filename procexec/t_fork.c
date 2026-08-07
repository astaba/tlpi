/* =========================================================================
 * Created on: <Thu Aug 06 16:19:26 +01 2026>
 * Time-stamp: <Thu Aug  6 16:20:30 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [file:../COPYING.gpl-v3] for details.
 * Desc      : ~/coding/c_prog/tlpi/procexec/t_fork.c -
 *
 * Listing 24.1: Demonstrate the use of fork(), showing that parent
 * and child get separate copies of stack and data segments.
 * ========================================================================= */
#include "tlpi_hdr.h"

static int idata = 111; /* Allocated in data segment */

int main(int argc, char *argv[]) {
  int istack = 222; /* Allocated in stack segment */
  pid_t childPid;

  switch (childPid = fork()) {
  case -1:
    systmErr("fork");

  case 0:
    idata *= 3;
    istack *= 3;
    break;

  default:
    sleep(3); /* Give child a chance to execute */
    break;
  }

  /* Both parent and child come here */

  printf("PID=%ld %s idata=%d istack=%d\n", (long)getpid(),
         (childPid == 0) ? "(child) " : "(parent)", idata, istack);

  exit(EXIT_SUCCESS);
}

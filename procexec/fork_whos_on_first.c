/* =========================================================================
 * Created on: <Thu Aug 06 16:24:04 +01 2026>
 * Time-stamp: <Thu Aug  6 16:24:54 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/procexec/fork_whos_on_first.c -
 *
 * Listing 24.5: Parent repeatedly creates a child, and then processes
 * both race to be the first to print a message. (Each child
 * terminates after printing its message.) The results of running this
 * program give us an idea of which of the two processes--parent or
 * child--is usually scheduled first after a fork().
 *
 * Whether the child or the parent is scheduled first after fork() has
 * changed a number of times across different kernel versions.
 * ========================================================================= */
#include "tlpi_hdr.h"
#include <sys/wait.h>

int main(int argc, char *argv[]) {
  int numChildren, j;
  pid_t childPid;

  if (argc > 1 && strcmp(argv[1], "--help") == 0)
    usageErr("%s [num-children]\n", argv[0]);

  numChildren = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-children") : 1;

  setbuf(stdout, NULL); /* Make stdout unbuffered */

  for (j = 0; j < numChildren; j++) {
    switch (childPid = fork()) {
    case -1:
      systmErr("fork");

    case 0:
      printf("%d child\n", j);
      _exit(EXIT_SUCCESS);

    default:
      printf("%d parent\n", j);
      wait(NULL); /* Wait for child to terminate */
      break;
    }
  }

  exit(EXIT_SUCCESS);
}

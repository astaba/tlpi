/* =========================================================================
 * Created on: <Thu Aug 06 16:23:11 +01 2026>
 * Time-stamp: <Thu Aug  6 16:23:53 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [file:../COPYING.gpl-v3] for details.
 * Desc      : ~/coding/c_prog/tlpi/procexec/t_vfork.c -
 *
 * Listing 24.4: Demonstrate the use of vfork() to create a child
 * process.
 * ========================================================================= */
#define _BSD_SOURCE /* To get vfork() declaration from <unistd.h>              \
                       in case _XOPEN_SOURCE >= 700 */
#include "tlpi_hdr.h"

int main(int argc, char *argv[]) {
  int istack = 222;

  switch (vfork()) {
  case -1:
    systmErr("vfork");

  case 0:     /* Child executes first, in parent's memory space */
    sleep(3); /* Even if we sleep for a while,
                 parent still is not scheduled */
    write(STDOUT_FILENO, "Child executing\n", 16);
    istack *= 3; /* This change will be seen by parent */
    _exit(EXIT_SUCCESS);

  default: /* Parent is blocked until child exits */
    write(STDOUT_FILENO, "Parent executing\n", 17);
    printf("istack=%d\n", istack);
    exit(EXIT_SUCCESS);
  }
}

/* =========================================================================
 * Created on: <Tue Aug 04 18:10:06 +01 2026>
 * Time-stamp: <Tue Aug  4 18:10:56 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [file:../COPYING.gpl-v3] for details.
 * Desc      : ~/coding/c_prog/tlpi/proc/display_env.c -
 *
 * Listing 6.3: Display the process environment list.
 * ========================================================================= */
#include "tlpi_hdr.h"

extern char **environ;
/* Or define _GNU_SOURCE to get it from <unistd.h> */

int main(int argc, char *argv[]) {
  char **ep;

  for (ep = environ; *ep != NULL; ep++)
    puts(*ep);

  exit(EXIT_SUCCESS);
}

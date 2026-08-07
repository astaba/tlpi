/* =========================================================================
 * Created on: <Tue Aug 04 18:07:23 +01 2026>
 * Time-stamp: <Tue Aug  4 18:08:54 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [file:../COPYING.gpl-v3] for details.
 * Desc      : ~/coding/c_prog/tlpi/proc/necho.c -
 *
 * Listing 6.2:
 * A simple version of echo(1): echo our command-line arguments.
 * ========================================================================= */
#include "tlpi_hdr.h"

int main(int argc, char *argv[]) {
  int j;

  for (j = 0; j < argc; j++)
    printf("argv[%d] = %s\n", j, argv[j]);

  exit(EXIT_SUCCESS);
}

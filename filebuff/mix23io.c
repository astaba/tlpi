/* =========================================================================
 * Created on: <Fri Jul 10 13:34:12 +01 2026>
 * Time-stamp: <Sun Jul 12 19:17:39 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/filebuff/mix23io.c -
 *
 * Supplementary program for Chapter 13
 *
 * Illustrates the impact of stdio buffering when using stdio library
 * functions and I/O system calls to work on the same file.
 *
 * Try running this program (with stdout directed to the terminal)
 * without and with a command-line argument (any string).
 * ========================================================================= */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("printf(): To man the world is twofold, ");

  if (argc > 1)
    printf("\n");

  write(STDOUT_FILENO, " write(): in accordance with his twofold attitude.\n",
        51);

  exit(EXIT_SUCCESS);
}

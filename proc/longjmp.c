/* =========================================================================
 * Created on: <Tue Aug 04 18:37:17 +01 2026>
 * Time-stamp: <Tue Aug  4 18:41:32 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [file:../COPYING.gpl-v3] for details.
 * Desc      : ~/coding/c_prog/tlpi/proc/longjmp.c -
 *
 * Listing 6.5: Demonstrate the use of setjmp() and longjmp() to
 * perform a nonlocal goto.
 *
 * Usage: longjmp [x]
 *
 * The presence or absence of a command-line argument determines which
 * of two functions (f1() or f2()) we will longjmp() from.
 * ========================================================================= */
#include "tlpi_hdr.h"
#include <setjmp.h>

static jmp_buf env;

static void f2(void) { longjmp(env, 2); }

static void f1(int argc) {
  if (argc == 1)
    longjmp(env, 1);
  f2();
}

int main(int argc, char *argv[]) {

  switch (setjmp(env)) {
  case 0: /* This is the return after the initial setjmp() */
    printf("Calling f1() after initial setjmp()\n");
    f1(argc); /* Never returns... */
    break;    /* ... but this is good form */

  case 1:
    printf("We jumped back from f1()\n");
    break;

  case 2:
    printf("We jumped back from f2()\n");
    break;
  }

  exit(EXIT_SUCCESS);
}

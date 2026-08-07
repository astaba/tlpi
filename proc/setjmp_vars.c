/* =========================================================================
 * Created on: <Tue Aug 04 19:05:25 +01 2026>
 * Time-stamp: <Tue Aug  4 19:06:49 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [file:../COPYING.gpl-v3] for details.
 * Desc      : ~/coding/c_prog/tlpi/proc/setjmp_vars.c -
 *
 * Listing 6.6: Compiling this program with and without optimization
 * yields different results, since the optimizer reorganizes code and
 * variables in a manner that does not take account of the dynamic
 * flow of control established by a long jump.
 *
 * Try looking at the assembler source (.s) for the unoptimized (cc
 * -S) and optimized (cc -O -S) versions of this program to see the
 * cause of these differences.
 * ========================================================================= */
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>

static jmp_buf env;

static void doJump(int nvar, int rvar, int vvar) {
  printf("Inside doJump(): nvar=%d rvar=%d vvar=%d\n", nvar, rvar, vvar);
  longjmp(env, 1);
}

int main(int argc, char *argv[]) {
  int nvar;
  register int rvar; /* Allocated in register if possible */
  volatile int vvar; /* See text */

  nvar = 111;
  rvar = 222;
  vvar = 333;

  if (setjmp(env) == 0) { /* Code executed after setjmp() */
    nvar = 777;
    rvar = 888;
    vvar = 999;
    doJump(nvar, rvar, vvar);

  } else { /* Code executed after longjmp() */

    printf("After longjmp(): nvar=%d rvar=%d vvar=%d\n", nvar, rvar, vvar);
  }

  exit(EXIT_SUCCESS);
}

/* =========================================================================
 * Created on: <Thu Aug 06 16:21:44 +01 2026>
 * Time-stamp: <Thu Aug  6 16:23:00 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/procexec/footprint.c -
 *
 * Listing 24.3:
 *
 * Using fork() + wait() to control the memory footprint of an
 * application.
 *
 * This program contains a function that (artificially) consumes a
 * large amount of memory. To avoid changing the process's memory
 * footprint, the program creates a child process that calls the
 * function. When the child terminates, all of its memory is freed,
 * and the memory consumption of the parent is left unaffected.
 * ========================================================================= */
#define _BSD_SOURCE /* To get sbrk() declaration from <unistd.h> in case       \
                       _XOPEN_SOURCE >= 600; defining _SVID_SOURCE or          \
                       _GNU_SOURCE also suffices */
#include "tlpi_hdr.h"
#include <sys/wait.h>

static int func(int arg) {
  int j;

  for (j = 0; j < 0x100; j++)
    if (malloc(0x8000) == NULL)
      systmErr("malloc");
  printf("Program break in child:  %10p\n", sbrk(0));

  return arg;
}

int main(int argc, char *argv[]) {
  int arg = (argc > 1) ? getInt(argv[1], 0, "arg") : 0;
  pid_t childPid;
  int status;

  setbuf(stdout, NULL); /* Disable buffering of stdout */

  printf("Program break in parent: %10p\n", sbrk(0));

  childPid = fork();
  if (childPid == -1)
    systmErr("fork");

  if (childPid == 0) /* Child calls func() and */
    exit(func(arg)); /* uses return value as exit status */

  /* Parent waits for child to terminate. It can determine the
     result of func() by inspecting 'status' */

  if (wait(&status) == -1)
    systmErr("wait");

  printf("Program break in parent: %10p\n", sbrk(0));

  printf("Status = %d %d\n", status, WEXITSTATUS(status));

  exit(EXIT_SUCCESS);
}

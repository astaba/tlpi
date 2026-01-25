/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2026.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU Lesser General Public License as published   *
* by the Free Software Foundation, either version 3 or (at your option)   *
* any later version. This program is distributed without any warranty.    *
* See the files COPYING.lgpl-v3 and COPYING.gpl-v3 for details.           *
\*************************************************************************/

/* Listing 3-3 */

/* error_functions.c
Some standard error handling routines used by various programs. */

/* Per the C standard, a "Normal Termination" via exit(3) should ideally occur
in serenity, performing the following: closing open streams, flushing
user-space buffers to the kernel, executing all registered atexit(3) handlers,
and terminating the process context gracefully.

Conversely, an "Abrupt Termination" bypasses these user-space cleanup
routines. This is achieved either by invoking _exit(2) to kill the process
immediately at the kernel level, or by calling abort(3), which raises SIGABRT
to terminate the process abnormally—potentially producing a core dump for
post-mortem analysis.

Irrespective of the "trumping power" or immediate cancellation effects of
these terminal calls, the core philosophy of this library is to preempt the
exit. By utilizing outputError() first, the library ensures that a diagnostic
output is flushed explicitly so that it is reliably captured by the kernel,
before the process state is subsequently destroyed by terminating it via
_exit(2) or abort(3). */

#include "error_functions.h"
#include "ename.c.inc" /* Defines ename and MAX_ENAME */
#include "tlpi_hdr.h"
#include <stdarg.h>

/* Dump core if EF_DUMPCORE environment variable is defined and
   is a nonempty string; otherwise call exit(3) or _exit(2),
   depending on the value of 'useExit3'. */
NORETURN
static void terminate(Boolean useExit3) {
  char *s;

  s = getenv("EF_DUMPCORE");

  if (s != NULL && *s != '\0')
    abort();
  else if (useExit3)
    exit(EXIT_FAILURE);
  else
    _exit(EXIT_FAILURE);
}

/* Diagnose 'errno' error by:

  * outputting a string containing the error name (if available
    in 'ename' array) corresponding to the value in 'err', along
    with the corresponding error message from strerror(), and

  * outputting the caller-supplied error message specified in
    'format' and 'ap'. */
static void outputError(Boolean useErr, int err, Boolean flushStdout,
                        const char *format, va_list ap) {
#define BUF_SIZE 500
  char buf[BUF_SIZE], userMsg[BUF_SIZE], errText[BUF_SIZE];

  vsnprintf(userMsg, BUF_SIZE, format, ap);

  if (useErr)
    snprintf(errText, BUF_SIZE, " [%s %s]",
             (err > 0 && err <= MAX_ENAME) ? ename[err] : "?UNKNOWN?",
             strerror(err));
  else
    snprintf(errText, BUF_SIZE, ":");

#if __GNUC__ >= 7
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
#endif
  snprintf(buf, BUF_SIZE, "ERROR%s %s\n", errText, userMsg);
#if __GNUC__ >= 7
#pragma GCC diagnostic pop
#endif

  if (flushStdout)
    fflush(stdout); /* Flush any pending stdout */
  fputs(buf, stderr);
  fflush(stderr); /* In case stderr is not line-buffered */
}

/* Print WARNING diagnostic: errno-based and custom error messages.
Preserve errno value and let the control flow. */
void errMsg(const char *format, ...) {
  va_list argList;
  int savedErrno;

  savedErrno = errno; /* In case we change it here */

  va_start(argList, format);
  outputError(TRUE, errno, TRUE, format, argList);
  va_end(argList);

  errno = savedErrno;
}

/* Print EXCEPTION diagnostic: errno-based and custom error messages.
If EF_DUMPCORE set: abort with SIGABRT and potential core dump.
Otherwise Cleanup and terminate: using exit(3). */
void errExit(const char *format, ...) {
  va_list argList;

  va_start(argList, format);
  outputError(TRUE, errno, TRUE, format, argList);
  va_end(argList);

  terminate(TRUE);
}

/* Print EXCEPTION diagnostic: errno-based and custom error messages.
  If EF_DUMPCORE set: abort with SIGABRT and potential core dump.
  Otherwise Terminate without cleanup: using _exit(2).

  The relationship between this function and errExit() is analogous
  to that between _exit(2) and exit(3): unlike errExit(), this
  function does not flush stdout and calls _exit(2) to terminate the
  process (rather than exit(3), which would cause exit handlers to be
  invoked).

  These differences make this function especially useful in a library
  function that creates a child process that must then terminate
  because of an error: the child must terminate without flushing
  stdio buffers that were partially filled by the caller and without
  invoking exit handlers that were established by the caller. */
void err_exit(const char *format, ...) {
  va_list argList;

  va_start(argList, format);
  outputError(TRUE, errno, FALSE, format, argList);
  va_end(argList);

  terminate(FALSE);
}

/* Print EXCEPTION diagnostic: user-provided-errnum and custom error messages.
If EF_DUMPCORE set: abort with SIGABRT and potential core dump.
Otherwise Cleanup and terminate: using exit(3). */
void errExitEN(int errnum, const char *format, ...) {
  va_list argList;

  va_start(argList, format);
  outputError(TRUE, errnum, TRUE, format, argList);
  va_end(argList);

  terminate(TRUE);
}

/* Print EXCEPTION diagnostic: custom error message only.
If EF_DUMPCORE set: abort with SIGABRT and potential core dump.
Otherwise Cleanup and terminate: using exit(3). */
void fatal(const char *format, ...) {
  va_list argList;

  va_start(argList, format);
  outputError(FALSE, 0, TRUE, format, argList);
  va_end(argList);

  terminate(TRUE);
}

/* Print EXCEPTION diagnostic:
   custom message related to command usage error.
   Cleanup and terminate: using exit(3). */
void usageErr(const char *format, ...) {
  va_list argList;

  fflush(stdout); /* Flush any pending stdout */

  fprintf(stderr, "Usage: ");
  va_start(argList, format);
  vfprintf(stderr, format, argList);
  va_end(argList);

  fflush(stderr); /* In case stderr is not line-buffered */
  exit(EXIT_FAILURE);
}

/* Print EXCEPTION diagnostic:
   custom message related to command-line arguments error.
   Cleanup and terminate: using exit(3). */
void cmdLineErr(const char *format, ...) {
  va_list argList;

  fflush(stdout); /* Flush any pending stdout */

  fprintf(stderr, "Command-line usage error: ");
  va_start(argList, format);
  vfprintf(stderr, format, argList);
  va_end(argList);

  fflush(stderr); /* In case stderr is not line-buffered */
  exit(EXIT_FAILURE);
}

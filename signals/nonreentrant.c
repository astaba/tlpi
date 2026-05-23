/* =========================================================================
 * Created on: <Sun May 17 14:58:08 +01 2026>
 * Time-stamp: <Sun May 17 19:06:56 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/signals/nonreentrant.c -
 *
 * Listing 21.1: Demonstrate the nonreentrant nature of some library
 * functions, in this example, crypt(3).
 * ========================================================================= */
#if !defined(_XOPEN_SOURCE) || _XOPEN_SOURCE < 600
#define _XOPEN_SOURCE 600
#endif
#include "../lib/tlpi_hdr.h"
#include <crypt.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

static char *str2;               /* Set from argv[2] */
static volatile int handled = 0; /* Counts number of calls to handler */

static void handler(int sig) {
  crypt(str2, "xx"); /* UNSAFE: */
  handled++;
}

int main(int argc, char *argv[]) {
  char *cr1;
  int callNum, mismatch;
  struct sigaction sa;

  if (argc != 3)
    usageErr("%s str1 str2\n", argv[0]);

  /* Make argv[2] available to handler */
  str2 = argv[2];
  /* Copy statically allocated string to another buffer */
  cr1 = strdup(crypt(argv[1], "xx"));
  if (cr1 == NULL)
    systmErr("strdup");

  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sa.sa_handler = handler;
  if (sigaction(SIGINT, &sa, NULL) == -1)
    systmErr("sigaction");

  /* NOTICE:(1) Repeatedly call crypt() using argv[1].  If interrupted
     by a signal handler right after crypt() returns but just before
     strcmp() reads the ponted memory space, then the static storage
     returned by crypt() will be overwritten by the results of
     encrypting argv[2], and strcmp() will detect a mismatch with the
     value in 'cr1'. */

  for (callNum = 1, mismatch = 0;; callNum++) {
    if (strcmp(crypt(argv[1], "xx"), cr1) != 0) {
      mismatch++;
      printf("Mismatch on call %d (mismatch=%d handled=%d)\n", callNum,
             mismatch, handled);
    }
  }
  /* NOTICE:(2) As you repeatedly press Ctrl-C and send SIGINT you
     will notice "handled" var oupace and getting greater then
     "mismatch" var.  The reason that it happens is strcmp() wins the
     race the read its first argument pointer returned by crypt() over
     the signal handler crypt() to overwrite it.  As a result there
     not mismatch (mismatch is not incremented) but in respons to
     SIGINT the handler runs and increment handled */
}

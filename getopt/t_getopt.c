/* =========================================================================
 * Created on: <Sun Jun 14 11:04:04 +01 2026>
 * Time-stamp: <Mon Jul 13 18:30:58 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/getopt/t_getopt.c -
 *
 * Listing B-1: Demonstrate the use of getopt(3) to parse command-line
 * options.
 * ========================================================================= */
#include "tlpi_hdr.h" /* IWYU pragma: keep */
#include <ctype.h>

#define printable(ch) (isprint((unsigned char)ch) ? ch : '#')

/* Prevent "this statement may fall through" warnings from "gcc
   -Wimplicit-fallthrough" in switch() statement in main(). */
#ifdef __GNUC__
__attribute__((noreturn))
#endif
static void /* Print "usage" message and exit */
usageError(char *progName, char *msg, int opt) {
  if (msg != NULL && opt != 0)
    fprintf(stderr, "%s (-%c)\n", msg, printable(opt));
  fprintf(stderr, "Usage: %s [-p arg] [-x]\n", progName);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  int opt, xfnd;
  char *pstr;

  xfnd = 0;
  pstr = NULL;

  while ((opt = getopt(argc, argv, ":p:x")) != -1) {
    printf("opt =%3d (%c); optind = %d", opt, printable(opt), optind);
    if (opt == '?' || opt == ':')
      printf("; optopt =%3d (%c)", optopt, printable(optopt));
    printf("\n");

    switch (opt) {
    case 'p':
      pstr = optarg;
      break;
    case 'x':
      xfnd++;
      break;
    case ':':
      usageError(argv[0], "Missing argument", optopt);
    case '?':
      usageError(argv[0], "Unrecognized option", optopt);
    default:
      custmErr("Unexpected case in switch()");
    }
  }

  if (xfnd != 0)
    printf("-x was specified (count=%d)\n", xfnd);
  if (pstr != NULL)
    printf("-p was specified with the value \"%s\"\n", pstr);
  if (optind < argc)
    printf("First nonoption argument is \"%s\" at argv[%d]\n", argv[optind],
           optind);
  exit(EXIT_SUCCESS);
}

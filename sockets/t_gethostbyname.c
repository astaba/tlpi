/* =========================================================================
 * Created on: <Sat May 09 22:29:01 +01 2026>
 * Time-stamp: <Sat May  9 22:30:16 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026
 *             See file [[file:../COPYING.gpl-v3]] for details GNU License.
 * Desc      : ~/coding/c_prog/tlpi/sockets/t_gethostbyname.c -
 *
 * Listing 59.10: Demonstrate the use of gethostbyname() to lookup of
 * the address for a given host name. Note that gethostbyname() is now
 * obsolete; new programs should use getaddrinfo().
 * ========================================================================= */
#if defined(_AIX) /* AIX 5.1 needs this to get hstrerror() declaration */
#define _USE_IRS
#endif
#define _BSD_SOURCE          /* To get hstrerror() declaration from <netdb.h> */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {
  struct hostent *h;
  char **pp;
  char str[INET6_ADDRSTRLEN];

  for (argv++; *argv != NULL; argv++) {
    h = gethostbyname(*argv);
    if (h == NULL) {
      fprintf(stderr, "gethostbyname() failed for '%s': %s\n", *argv,
              hstrerror(h_errno));
      continue;
    }

    printf("Canonical name: %s\n", h->h_name);

    printf("        alias(es):     ");
    for (pp = h->h_aliases; *pp != NULL; pp++)
      printf(" %s", *pp);
    printf("\n");

    printf("        address type:   %s\n",
           (h->h_addrtype == AF_INET)    ? "AF_INET"
           : (h->h_addrtype == AF_INET6) ? "AF_INET6"
                                         : "???");

    if (h->h_addrtype == AF_INET || h->h_addrtype == AF_INET6) {
      printf("        address(es):   ");
      for (pp = h->h_addr_list; *pp != NULL; pp++)
        printf(" %s", inet_ntop(h->h_addrtype, *pp, str, INET6_ADDRSTRLEN));
      printf("\n");
    }
  }

  exit(EXIT_SUCCESS);
}

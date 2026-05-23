/* =========================================================================
 * Created on: <Sat May 09 22:59:32 +01 2026>
 * Time-stamp: <Mon May 11 00:01:45 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/yt_gethostbyname.c -
 *
 * Listing 59.10: Using gethostbyname() to retrieve host information
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char *argv[argc + 1]) {
  struct hostent *h;
  char **cv, buf[INET6_ADDRSTRLEN];

  for (argv++; *argv != NULL; argv++) {
    h = gethostbyname(*argv);
    if (h == NULL) {
      fprintf(stderr, "ERROR gethostbyname() failed: %s\n", hstrerror(h_errno));
      printf("\n");
      continue;
    }

    printf("%16s%s\n", "Canonical name: ", h->h_name);

    printf("%16s", "alias(es): ");
    for (cv = h->h_aliases; *cv != NULL; cv++) {
      printf("%s", *cv);
      if (*(cv + 1) != NULL)
        printf("\n%16s", "");
    }
    printf("\n");

    printf("%16s%s\n", "address type: ",
           (h->h_addrtype == AF_INET)    ? "AF_INET"
           : (h->h_addrtype == AF_INET6) ? "AF_INET6"
                                         : "???");

    if ((h->h_addrtype == AF_INET) || (h->h_addrtype == AF_INET6)) {
      printf("%16s", "address(es): ");
      for (cv = h->h_addr_list; *cv != NULL; cv++) {
        printf("%s", inet_ntop(h->h_addrtype, *cv, buf, INET6_ADDRSTRLEN));
        if (*(cv + 1) != NULL)
          printf("\n%16s", "");
      }
    }
    printf("\n\n");
  }

  return EXIT_SUCCESS;
}

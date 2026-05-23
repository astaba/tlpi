/* =========================================================================
 * Created on: <Sun May 10 00:30:38 +01 2026>
 * Time-stamp: <Sun May 10 00:32:06 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026
 *             See file [[file:../COPYING.gpl-v3]] for details GNU License.
 * Desc      : ~/coding/c_prog/tlpi/sockets/t_getservbyname.c -
 *
 * Supplementary program for Chapter 59: Demonstrate the use of
 * getservbyname() to look up the port number corresponding to a given
 * service name. Note that getservbyname() is now obsolete; new
 * programs should use getaddrinfo().
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <netdb.h>

int main(int argc, char *argv[]) {
  if (argc < 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s service [protocol]\n", argv[0]);

  struct servent *s = getservbyname(argv[1], argv[2]);
  if (s == NULL)
    custmErr("getservbyname() could not find a matching record");

  printf("Official name: %s\n", s->s_name);
  printf("Aliases:      ");
  for (char **pp = s->s_aliases; *pp != NULL; pp++)
    printf(" %s", *pp);
  printf("\n");
  printf("Port:          %u\n", ntohs(s->s_port));
  printf("Protocol:      %s\n", s->s_proto);

  exit(EXIT_SUCCESS);
}

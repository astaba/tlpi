/* =========================================================================
 * Created on: <Mon Apr 13 02:47:02 +01 2026>
 * Time-stamp: <Tue Apr 14 03:07:26 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026
 *             See file [[file:../COPYING.gpl-v3]] for details GNU License.
 * Desc      : ~/coding/c_prog/tlpi/sockets/is_seqnum_cl.c -
 *
 * Listing 59.7 A simple Internet stream socket client. This client
 * requests a sequence number from the server.
 *
 * See also [[file:is_seqnum_sv.c]].
 * ========================================================================= */
#include "is_seqnum.h"
#include <netdb.h>

int main(int argc, char *argv[]) {
  char *reqLenStr;         /* Requested length of sequence */
  char seqNumStr[INT_LEN]; /* Start of granted sequence */
  int cfd;
  ssize_t numRead;
  struct addrinfo hints;
  struct addrinfo *result, *rp;

  if (argc < 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s server-host [sequence-len]\n", argv[0]);

  /* Call getaddrinfo() to obtain a list of addresses that
     we can try connecting to */

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;
  hints.ai_family = AF_UNSPEC; /* Allows IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_NUMERICSERV;
  /* (1) */
  if (getaddrinfo(argv[1], PORT_NUM, &hints, &result) != 0)
    systmErr("getaddrinfo");

  /* Walk through returned list until we find an address structure
     that can be used to successfully connect a socket */
  /* (2) */
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    /* (3) */
    cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (cfd == -1)
      continue; /* On error, try next address */
    /* (4) */
    if (connect(cfd, rp->ai_addr, rp->ai_addrlen) == 0)
      break; /* Success */
    /* Connect failed: close this socket and try next address */
    close(cfd);
  }

  if (rp == NULL)
    custmErr("Could not connect socket to any address");
  freeaddrinfo(result);

  /* Send requested sequence length, with terminating newline */
  /* (5) */
  reqLenStr = (argc > 2) ? argv[2] : "1";
  if (write(cfd, reqLenStr, strlen(reqLenStr)) != (ssize_t)strlen(reqLenStr))
    custmErr("Partial/failed write (reqLenStr)");
  if (write(cfd, "\n", 1) != 1)
    custmErr("Partial/failed write (newline)");

  /* Read and display sequence number returned by server */
  /* (6) */
  numRead = readLine(cfd, seqNumStr, INT_LEN);
  if (numRead == -1)
    systmErr("readLine");
  if (numRead == 0)
    custmErr("Unexpected EOF from server");
  /* (7) */
  printf("Sequence number: %s", seqNumStr); /* Includes '\n' */

  exit(EXIT_SUCCESS); /* Closes 'cfd' */
}

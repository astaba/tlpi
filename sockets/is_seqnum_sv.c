/* =========================================================================
 * Created on: <Mon Apr 13 02:36:30 +01 2026>
 * Time-stamp: <Mon Apr 13 22:45:01 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026
 *             See file [[file:../COPYING.gpl-v3]] for details GNU License.
 * Desc      : ~/coding/c_prog/tlpi/sockets/is_seqnum_sv.c -
 *
 * Listing 59.6 A simple Internet stream socket server. Our service is
 * to provide unique sequence numbers to clients.
 *
 * Usage:  is_seqnum_sv [init-seq-num]
 *                      (default = 0)
 *
 * See also [[file:is_seqnum_cl.c]].
 * ========================================================================= */
/* To get definitions of NI_MAXHOST and NI_MAXSERV from <netdb.h> */
#define _GNU_SOURCE

#include "is_seqnum.h"
#include <netdb.h>

#define BACKLOG 50

int main(int argc, char *argv[]) {
  uint32_t seqNum;
  char reqLenStr[INT_LEN]; /* Length of requested sequence */
  char seqNumStr[INT_LEN]; /* Start of granted sequence */
  struct sockaddr_storage claddr;
  int lfd, cfd, optval, reqLen;
  socklen_t addrlen;
  struct addrinfo hints;
  struct addrinfo *result, *rp;
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
  char addrStr[ADDRSTRLEN];
  char host[NI_MAXHOST];
  char service[NI_MAXSERV];

  if (argc > 1 && strcmp(argv[1], "--help") == 0)
    usageErr("%s [init-seq-num]\n", argv[0]);
  /* (1) */
  seqNum = (argc > 1) ? getInt(argv[1], 0, "init-seq-num") : 0;

  /* Ignore the SIGPIPE signal, so that we find out about broken connection
     errors via a failure from write(). */
  /* (2) */
  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    systmErr("signal");

  /* Call getaddrinfo() to obtain a list of addresses that
     we can try binding to */

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_UNSPEC; /* Allows IPv4 or IPv6 */
  /* (3) */
  hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
  /* Wildcard IP address; service name is numeric */
  /* (4) */
  if (getaddrinfo(NULL, PORT_NUM, &hints, &result) != 0)
    systmErr("getaddrinfo");

  /* Walk through returned list until we find an address structure
     that can be used to successfully create and bind a socket */

  optval = 1;
  /* (5) */
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (lfd == -1)
      continue; /* On error, try next address */
    /* (6) */
    if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) ==
        -1)
      systmErr("setsockopt");
    /* (7) */
    if (bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0)
      break; /* Success */

    /* bind() failed: close this socket and try next address */

    close(lfd);
  }
  if (rp == NULL)
    custmErr("Could not bind socket to any address");
  
  /* (8) */
  if (listen(lfd, BACKLOG) == -1)
    systmErr("listen");

  freeaddrinfo(result);
  /* (9) */
  for (;;) { /* Handle clients iteratively */
    /* Accept a client connection, obtaining client's address */
    addrlen = sizeof(struct sockaddr_storage);
    /* (10) */
    cfd = accept(lfd, (struct sockaddr *)&claddr, &addrlen);
    if (cfd == -1) {
      systmWrn("accept");
      continue;
    }
    /* (11) */
    if (getnameinfo((struct sockaddr *)&claddr, addrlen, host, NI_MAXHOST,
                    service, NI_MAXSERV, 0) == 0)
      snprintf(addrStr, ADDRSTRLEN, "(%s, %s)", host, service);
    else
      snprintf(addrStr, ADDRSTRLEN, "(?UNKNOWN?)");
    printf("Connection from %s\n", addrStr);

    /* Read client request, send sequence number back */
    /* (12) */
    if (readLine(cfd, reqLenStr, INT_LEN) <= 0) {
      close(cfd);
      continue; /* Failed read; skip request */
    }
    /* (13) */
    reqLen = atoi(reqLenStr);
    if (reqLen <= 0) { /* Watch for misbehaving clients */
      close(cfd);
      continue; /* Bad request; skip it */
    }
    /* (14) */
    snprintf(seqNumStr, INT_LEN, "%d\n", seqNum);
    if (write(cfd, seqNumStr, strlen(seqNumStr)) != (ssize_t)strlen(seqNumStr))
      fprintf(stderr, "Error on write");
    /* (15) */
    seqNum += reqLen; /* Update sequence number */

    if (close(cfd) == -1) /* Close connection */
      systmWrn("close");
  }
}

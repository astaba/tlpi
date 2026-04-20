/* =========================================================================
 * Created on: <Sat Apr 18 11:57:15 +01 2026>
 * Time-stamp: <Mon Apr 20 16:59:05 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/exr5901_sv.c -
 *
 * Exercise 59.1
 * Implement buffered I/O in Listing 59.6 [[file:is_seqnum_sv.c]]
 * For details see [[file:README.org::#exercise-59-1]].
 * ========================================================================= */
#define _GNU_SOURCE

#include "is_seqnum.h"
#include "ybio.h"
#include <netdb.h>

#define BACKLOG 50

int main(int argc, char *argv[argc + 1]) {
  int seqNum, rc, optval, lisnfd, connfd, reqLen;
  struct addrinfo hints, *aiResult, *ptr;
  socklen_t clSoLen;
  struct sockaddr_storage clSoAddr;
  char clHostNi[NI_MAXHOST], clServNi[NI_MAXSERV];
#define WHOLE_NI_BUF ((((NI_MAXHOST + NI_MAXSERV) / 4096) * 4096) + 4096)
  char clWholeNi[WHOLE_NI_BUF];
  char reqLenStr[INT_LEN]; /* Length of requested sequence */
  char seqNumStr[INT_LEN]; /* Start of granted sequence */
  ybio_t bio;

  if (argc > 1 && !strcmp(argv[1], "--help"))
    usageErr("%s [init-seq-num]\n", argv[0]);
  seqNum = (argc > 1) ? getInt(argv[1], 0, "init-seq-num") : 0;

  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    systmErr("signal() failed");

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
  /* Don't bother about the remaining fields */
  /* hints.ai_protocol = 0; // With 0 the appropriate protocol is set by the
   * system based on the preceding af_family and socktype */
  /* hints.ai_canonname = NULL; */
  /* hints.ai_addrlen = 0; */

  rc = getaddrinfo(NULL, PORT_NUM, &hints, &aiResult);
  if (rc) {
    fprintf(stderr, "getaddrinfo() failed: %s\n", gai_strerror(errno));
    exit(EXIT_FAILURE);
  }

  optval = 1;
  for (ptr = aiResult; ptr; ptr = ptr->ai_next) {
    lisnfd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (lisnfd == -1)
      continue; /* socket() failed, try another address */
    /* We have a valid socket file descriptor. Modify it before bind() */
    if (setsockopt(lisnfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) !=
        0) {
      systmErr("setsockopt() failed");
    }

    if (bind(lisnfd, ptr->ai_addr, ptr->ai_addrlen) == 0)
      break; /* Success */
    /* bind() close socket fd before trying another address */
    if (close(lisnfd) == -1)
      systmErr("close() failed");
  } /* Walk through completed: make sure not a complete failure */
  if (ptr == NULL)
    systmErr("Could not find any address");
  freeaddrinfo(aiResult);

  /* Make socket passive */
  if (listen(lisnfd, BACKLOG) == -1)
    systmErr("listen() failed");

  /* Start iterative loop */
  for (;;) {
    clSoLen = sizeof(struct sockaddr_storage);
    connfd = accept(lisnfd, (struct sockaddr *)&clSoAddr, &clSoLen);
    if (connfd == -1) {
      systmWrn("accept() failed");
      continue;
    }

    /* Initialize state monitor structure for buffered I/O */
    ybio_init(connfd, &bio);

    /* (11) We have a connected client: retrieve address and port */
    rc = getnameinfo((struct sockaddr *)&clSoAddr, clSoLen, clHostNi,
                     NI_MAXHOST, clServNi, NI_MAXSERV, 0);
    if (rc)
      snprintf(clWholeNi, WHOLE_NI_BUF, "[?UNKNOWN?] getnameinfo() failed: %s",
               gai_strerror(errno));
    else
      snprintf(clWholeNi, WHOLE_NI_BUF, "(%s, %s)", clHostNi, clServNi);
    printf("Yis_SV: Connection from: %s\n", clWholeNi);

    /* (12) Read one data line from the client */
    if ((rc = ybio_readl(&bio, reqLenStr, INT_LEN)) <= 0) {
      if (rc < 0)
        fprintf(stderr, "yreadLine() failed: %s\n", strerror(errno));
      if (close(connfd) == -1)
        systmErr("close() failed");
      continue;
    }

    /* (13) Parse client string into interger */
    rc = tryGetInt(reqLenStr, TGN_IGN_TRAILWS, "int-req-num", &reqLen);
    if (rc == TGN_FAILURE) { /* Parsing failed: Drop client and continue */
      if (close(connfd) == -1)
        systmErr("close() failed");
      continue; /* tryGetInt() already reported the precise error condition */
    }

    /* (14) Format seqNum into response string and send to client */
    snprintf(seqNumStr, INT_LEN, "%d\n", seqNum);
    if (write(connfd, seqNumStr, strlen(seqNumStr)) !=
        (ssize_t)strlen(seqNumStr)) {
      if (errno == EPIPE)
        fprintf(stderr, "Client quit prematurely: %s\n", strerror(errno));
      else
        fprintf(stderr, "partial/failed write()\n");
    }

    /* (15) Update sequence number */
    seqNum += reqLen;

    if (close(connfd) == -1) /* Close connection */
      systmErr("close() failed");
  }
}

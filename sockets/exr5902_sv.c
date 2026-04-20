/* =========================================================================
 * Created on: <Sat Apr 18 19:00:02 +01 2026>
 * Time-stamp: <Sat Apr 18 20:44:33 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/exr5902_sv.c -
 *
 * Exercise 59.2
 * For details see [[file:README.org::#exercise-59-2]]
 * [[file:exr5902_cl.c]]
 * See [[file:is_seqnum_sv.c]], [[file:inet_sockets.c]]
 * ========================================================================= */
#define _GNU_SOURCE

#include "inet_sockets.h" /* Declares functions defined here */
#include "is_seqnum.h"
#include "ybio.h"
#include <netdb.h>

#define BACKLOG 50

int main(int argc, char *argv[argc + 1]) {
  int seqNum, rc, lisnfd, connfd, reqLen;
  socklen_t clSoLen;
  ybio_t bio;
  struct sockaddr_storage clSoAddr;
#define WHOLE_NI_BUF ((((NI_MAXHOST + NI_MAXSERV) / 4096) * 4096) + 4096)
  char clWholeNi[WHOLE_NI_BUF];
  char reqLenStr[INT_LEN]; /* Length of requested sequence */
  char seqNumStr[INT_LEN]; /* Start of granted sequence */

  if (argc > 1 && !strcmp(argv[1], "--help"))
    usageErr("%s [init-seq-num]\n", argv[0]);
  seqNum = (argc > 1) ? getInt(argv[1], 0, "init-seq-num") : 0;

  if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    systmErr("signal() failed");

  /* Create TCP socket fd for multihomed listening */
  lisnfd = inetListen(PORT_NUM, BACKLOG, NULL);

  /* Start iterative server transaction loop */
  for (;;) {
    clSoLen = sizeof(struct sockaddr_storage);
    connfd = accept(lisnfd, (struct sockaddr *)&clSoAddr, &clSoLen);
    if (connfd == -1) {
      systmWrn("accept() failed");
      continue;
    }

    /* Initialize struct to monitor buffered I/O */
    ybio_init(connfd, &bio);

    /* (11) We have a connected client: retrieve address and port */
    inetAddressStr((struct sockaddr *)&clSoAddr, clSoLen, clWholeNi,
                   WHOLE_NI_BUF);
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
    if (rc != TGN_SUCCESS) { /* Parsing failed: Drop client and continue */
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

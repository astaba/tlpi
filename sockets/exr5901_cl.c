/* =========================================================================
 * Created on: <Sat Apr 18 11:49:10 +01 2026>
 * Time-stamp: <Mon Apr 20 16:59:05 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/exr5901_cl.c -
 *
 * Exercise 59.1
 * Implement buffered I/O in Listing 59.7 [[file:is_seqnum_cl.c]]
 * For details see [[file:README.org::#exercise-59-1]].
 * ========================================================================= */
#include "is_seqnum.h"
#include "ybio.h"
#include <netdb.h>

int main(int argc, char *argv[argc + 1]) {
  struct addrinfo hints, *niResult, *pt;
  int sofd, rc;
  char *reqNumStr = NULL, seqNumStr[INT_LEN];
  ssize_t numRead;
  ybio_t bio;

  if (argc < 2 || !strcmp(argv[1], "--help")) {
    usageErr("%s <peer-host-name> [request-number]\n", argv[0]);
  };

  memset(&hints, 0, sizeof(struct sockaddr));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_NUMERICSERV;

  rc = getaddrinfo(argv[1], PORT_NUM, &hints, &niResult);
  if (rc)
    systmErr("getaddrinfo() failed");

  /* Walk the linked list */
  for (pt = niResult; pt; pt = pt->ai_next) {
    sofd = socket(pt->ai_family, pt->ai_socktype, pt->ai_protocol);
    if (sofd == -1)
      continue;
    if (connect(sofd, pt->ai_addr, pt->ai_addrlen) == 0)
      break; /* Success */
    if (close(sofd) == -1)
      systmErr("close() failed");
  }

  /* Initialize state monitor structure for buffered I/O */
  ybio_init(sofd, &bio);

  if (argc > 2) {
    if (tryGetInt(argv[2], GN_GT_0, "req-num-str", NULL) == TGN_SUCCESS)
      reqNumStr = argv[2];
  } else
    reqNumStr = "1";

  if (write(sofd, reqNumStr, strlen(reqNumStr)) != (ssize_t)strlen(reqNumStr))
    custmErr("partial/failed wrote() \"req-num-str\"");
  if (write(sofd, "\n", 1) != 1)
    custmErr("partial/failed wrote() newline");

  numRead = ybio_readl(&bio, seqNumStr, INT_LEN);
  if (numRead < 0)
    systmErr("yreadLine() failed");
  if (numRead == 0)
    custmErr("Unexpected EOF from server");

  printf("Sequence length: %.*s", (int)numRead, seqNumStr);

  exit(EXIT_SUCCESS);
}

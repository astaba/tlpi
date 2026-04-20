/* =========================================================================
 * Created on: <Sat Apr 18 19:04:13 +01 2026>
 * Time-stamp: <Sat Apr 18 20:22:57 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/exr5902_cl.c -
 *
 * Exercise 59.2
 * For details see [[file:README.org::#exercise-59-2]]
 * [[file:exr5902_sv.c]]
 * See [[file:is_seqnum_sv.c]], [[file:inet_sockets.c]]
 * ========================================================================= */
#include "inet_sockets.h" /* Declares functions defined here */
#include "is_seqnum.h"
#include "ybio.h"
#include <netdb.h>

int main(int argc, char *argv[argc + 1]) {
  int sofd;
  char *reqNumStr = NULL, seqNumStr[INT_LEN];
  ssize_t numRead;
  ybio_t bio;

  /* Validate command line arguments */
  if (argc < 2 || !strcmp(argv[1], "--help")) {
    usageErr("%s <peer-host-name> [request-number]\n", argv[0]);
  };

  /* Create TCP socket fd connected to peer server */
  sofd = inetConnect(argv[1], PORT_NUM, SOCK_STREAM);

  /* Init struct to monitor buffered I/O */
  ybio_init(sofd, &bio);

  /* Initialize request number string */
  if (argc > 2) {
    if (tryGetInt(argv[2], GN_GT_0, "req-num-str", NULL) == TGN_SUCCESS)
      reqNumStr = argv[2];
  } else
    reqNumStr = "1";

  /* Socket Output stream */
  if (write(sofd, reqNumStr, strlen(reqNumStr)) != (ssize_t)strlen(reqNumStr))
    custmErr("partial/failed wrote() \"req-num-str\"");
  if (write(sofd, "\n", 1) != 1)
    custmErr("partial/failed wrote() newline");

  /* Socket Input stream */
  numRead = ybio_readl(&bio, seqNumStr, INT_LEN);
  if (numRead < 0)
    systmErr("ybio_readl() failed");
  if (numRead == 0)
    custmErr("Unexpected EOF from server");
  /* Feedback socket input to stdout */
  printf("Sequence length: %.*s", (int)numRead, seqNumStr);

  exit(EXIT_SUCCESS);
}

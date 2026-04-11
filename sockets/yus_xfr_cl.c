/* =========================================================================
 * Created on: <Tue Apr 07 17:51:54 +01 2026>
 * Time-stamp: <Wed Apr  8 14:10:14 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/yus_xfr_cl.c -
 * Sandbox for [[file:us_xfr_cl.c]]
 * See server at [[file:yus_xfr_sv.c]]
 * ========================================================================= */
#include "us_xfr.h"

int main(int argc, char *argv[argc + 1]) {
  int clfd;
  struct sockaddr_un peerAddr;
  ssize_t numRead;
  char buf[BUFSIZ];

  /* (1) Create socket file descriptor for the client */
  if ((clfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    systmErr("socket() failed");

  /* (2) Construct struct sockaddr_un for the remote address. */
  memset(&peerAddr, 0, sizeof(struct sockaddr_un));
  peerAddr.sun_family = AF_UNIX;
  strncpy(peerAddr.sun_path, SV_SOCK_PATH, sizeof(peerAddr.sun_path) - 1);

  /* (3) Connect to remote server (dial its number and call) */
  if (connect(clfd, (struct sockaddr *)&peerAddr, sizeof(struct sockaddr_un)) ==
      -1)
    systmErr("connect() failed");

  /* (4) Manage TCP I/O */
  while ((numRead = read(STDIN_FILENO, buf, BUFSIZ)) > 0)
    if (write(clfd, buf, numRead) != numRead)
      custmErr("partial/failed write()");

  if (numRead == -1)
    systmErr("read() failed");

  /* (5) The graceful exit closes socket then server sees EOF. */
  exit(EXIT_SUCCESS);
}

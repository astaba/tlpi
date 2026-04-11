/* =========================================================================
 * Created on: <Tue Apr 07 15:53:30 +01 2026>
 * Time-stamp: <Tue Apr  7 15:57:48 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026
 *             See file COPYING.gpl-v3 for details GNU License.
 * Desc      : ~/coding/c_prog/tlpi/sockets/us_xfr_cl.c -
 * Listing 57.4 An example UNIX domain stream socket client.
 * This client transmits contents of stdin to a server socket.
 * See server at [[file:us_xfr_sv.c]]
 * ========================================================================= */
#include "us_xfr.h"

int main(int argc, char *argv[]) {
  struct sockaddr_un addr;
  int sfd;
  ssize_t numRead;
  char buf[BUF_SIZE];

  sfd = socket(AF_UNIX, SOCK_STREAM, 0); /* Create client socket */
  if (sfd == -1)
    systmErr("socket");

  /* Construct server address, and make the connection */

  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

  if (connect(sfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1)
    systmErr("connect");

  /* Copy stdin to socket */

  while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
    if (write(sfd, buf, numRead) != numRead)
      custmErr("partial/failed write");

  if (numRead == -1)
    systmErr("read");

  exit(EXIT_SUCCESS); /* Closes our socket; server sees EOF */
}

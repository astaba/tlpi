/* =========================================================================
 * Created on: <Sun Apr 19 13:47:35 +01 2026>
 * Time-stamp: <Mon Apr 20 14:12:52 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/exr5903_us_xfr_cl.c -
 *
 * Exercise 59.3 Applying [[file:yunix_sockets.c]] library to Listing 57.4
 * [[file:us_xfr_cl.c]] An example UNIX domain stream socket client.
 * ========================================================================= */
#include "unix_sockets.h"
#include "us_xfr.h"

int main(int argc, char *argv[]) {
  struct sockaddr_un peeraddr;
  int sfd;
  ssize_t numRead;
  char buf[BUF_SIZE];

  /* Create a UNIX SOCK_STREAM socket fd and connect() to peer sockaddr_un */
  sfd = yunixconnect(SV_SOCK_PATH, &peeraddr, SOCK_STREAM);
  if (sfd == -1)
    systmErr("yunixconnect() failed");

  /* Copy stdin to socket */

  while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
    if (write(sfd, buf, numRead) != numRead)
      custmErr("partial/failed write");

  if (numRead == -1)
    systmErr("read");

  exit(EXIT_SUCCESS); /* Closes our socket; server sees EOF */
}

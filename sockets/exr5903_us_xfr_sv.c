/* =========================================================================
 * Created on: <Sun Apr 19 13:37:23 +01 2026>
 * Time-stamp: <Mon Apr 20 14:13:04 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/exr5903_us_xfr_sv.c -
 *
 * Exercise 59.3 Applying [[file:yunix_sockets.c]] library to Listing 57.3
 * [[file:us_xfr_sv.c]] An example UNIX stream socket server.
 * ========================================================================= */
#include "unix_sockets.h"
#include "us_xfr.h"

#define BACKLOG 5

int main(int argc, char *argv[]) {
  struct sockaddr_un localaddr;
  int lisnfd, connfd;
  ssize_t numRead;
  char buf[BUF_SIZE];

  /* Create a UNIX SOCK_STREAM socket fd and bind() to local sockaddr_un */
  lisnfd = yunixbind(SV_SOCK_PATH, &localaddr, SOCK_STREAM);
  if (lisnfd == -1)
    systmErr("yunixbind() failed");

  if (listen(lisnfd, BACKLOG) == -1)
    systmErr("listen");

  for (;;) { /* Handle client connections iteratively */
    /* Accept a connection. The connection is returned on a new
       socket, 'cfd'; the listening socket ('sfd') remains open
       and can be used to accept further connections. */

    if ((connfd = accept(lisnfd, NULL, NULL)) == -1)
      systmErr("accept");

    /* Transfer data from connected socket to stdout until EOF */

    while ((numRead = read(connfd, buf, BUF_SIZE)) > 0)
      if (write(STDOUT_FILENO, buf, numRead) != numRead)
        custmErr("partial/failed write");

    if (numRead == -1)
      systmErr("read");

    if (close(connfd) == -1)
      systmWrn("close");
  }
}

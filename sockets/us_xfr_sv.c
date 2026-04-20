/* =========================================================================
 * Created on: <Tue Apr 07 13:28:26 +01 2026>
 * Time-stamp: <Sun Apr 19 13:51:15 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026
 *             See file [[file:../COPYING.gpl-v3]] for details GNU License.
 * Desc      : ~/coding/c_prog/tlpi/sockets/us_xfr_sv.c -
 * Listing 57.3: An example UNIX stream socket server.
 * Accepts incoming connections and copies data sent from clients to stdout.
 * See client at [[file:us_xfr_cl.c]]
 * ========================================================================= */
#include "us_xfr.h"
#define BACKLOG 5

int main(int argc, char *argv[]) {
  struct sockaddr_un addr;
  int sfd, cfd;
  ssize_t numRead;
  char buf[BUF_SIZE];

  if ((sfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    systmErr("socket");

  /* Construct server socket address, bind socket to it,
     and make this a listening socket */

  /* HACK: Follow this link for explanation on both following checks:
     [[file:README.org::#stream-sockets-in-the-unix-domain]] */
  if (strlen(SV_SOCK_PATH) > sizeof(addr.sun_path) - 1)
    custmErr("Server socket path too long: %s", SV_SOCK_PATH);
  /* WARN: Make sure path didn't exist before being created anew. */
  if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT)
    systmErr("remove-%s", SV_SOCK_PATH);

  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

  if (bind(sfd, (struct sockaddr *)&addr, SUN_LEN(&addr)) == -1)
    systmErr("bind");

  if (listen(sfd, BACKLOG) == -1)
    systmErr("listen");

  for (;;) { /* Handle client connections iteratively */
    /* Accept a connection. The connection is returned on a new
       socket, 'cfd'; the listening socket ('sfd') remains open
       and can be used to accept further connections. */

    if ((cfd = accept(sfd, NULL, NULL)) == -1)
      systmErr("accept");

    /* Transfer data from connected socket to stdout until EOF */

    while ((numRead = read(cfd, buf, BUF_SIZE)) > 0)
      if (write(STDOUT_FILENO, buf, numRead) != numRead)
        custmErr("partial/failed write");

    if (numRead == -1)
      systmErr("read");

    if (close(cfd) == -1)
      systmWrn("close");
  }
}

/* =========================================================================
 * Created on: <Mon Apr 20 14:17:45 +01 2026>
 * Time-stamp: <Mon Apr 20 14:35:47 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/exr5903_a_us_xfr_sv.c -
 *
 * Exercise 59.3 Apply [[file:yunix_sockets.c]] library to
 * [[file:exr5702_sv.c]] which is applying the abstract namespace API
 * from that library to [[file:us_xfr_sv.c]]
 * See [[file:exr5903_a_us_xfr_cl.c]]
 * ========================================================================= */
#include "unix_sockets.h"
#include "us_xfr.h"

#define BACKLOG 5

int main(int argc, char *argv[argc + 1]) {
  int sv_fd, confd;
  struct sockaddr_un localAddr;
  ssize_t numRead;
  char buf[BUF_SIZE];

  if ((sv_fd = yunixAbind(SV_SOCK_PATH, &localAddr, SOCK_STREAM)) == -1)
    systmErr("yunixAbind() failed");

  if (listen(sv_fd, BACKLOG) == -1)
    systmErr("listen() failed");

  while (1) {
    confd = accept(sv_fd, NULL, NULL);
    if (confd == -1)
      systmErr("accept() failed");

    while ((numRead = read(confd, buf, BUF_SIZE)) > 0)
      if (write(STDOUT_FILENO, buf, numRead) != numRead)
        custmErr("partial/failed write()");

    if (numRead == -1)
      systmErr("read() failed");

    if (close(confd) == -1)
      systmErr("close() failed");
  }
}

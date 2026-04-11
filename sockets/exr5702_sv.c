/* =========================================================================
 * Created on: <Wed Apr 08 16:09:22 +01 2026>
 * Time-stamp: <Wed Apr  8 23:06:28 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/yus_axfr_sv_ex_57_2.c -
 * Server for Exercise 57.2 Rewrite the programs in Listing 57-3
 * [[file:us_xfr_sv.c]] and Listing 57-4 [[file:us_xfr_cl.c]]
 * to use the Linux-specific abstract socket namespace (Section 57.6).
 * See peer at [[file:exr5702_cl.c]]
 * ========================================================================= */
#include "us_xfr.h"
#define BACKLOG 5

int main(int argc, char *argv[argc + 1]) {
  int sv_fd, confd;
  struct sockaddr_un localAddr;
  ssize_t numRead;
  char buf[BUF_SIZE];

  if ((sv_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    systmErr("socket() failed");

  memset(&localAddr, 0, sizeof(struct sockaddr_un));
  localAddr.sun_family = AF_UNIX;
  strncpy(localAddr.sun_path + 1, SV_SOCK_PATH, sizeof(localAddr.sun_path) - 2);

  if (bind(sv_fd, (struct sockaddr *)&localAddr, sizeof(struct sockaddr_un)) ==
      -1)
    systmErr("bind() failed");

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

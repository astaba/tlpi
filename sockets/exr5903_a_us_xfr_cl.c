/* =========================================================================
 * Created on: <Mon Apr 20 14:27:38 +01 2026>
 * Time-stamp: <Mon Apr 20 14:34:59 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/exr5903_a_us_xfr_cl.c -
 *
 * Exercise 59.3 Apply [[file:yunix_sockets.c]] library to
 * [[file:exr5702_cl.c]] which is applying the abstract namespace API
 * from that library to [[file:us_xfr_cl.c]].
 * See [[file:exr5903_a_us_xfr_sv.c]]
 * ========================================================================= */
#include "unix_sockets.h"
#include "us_xfr.h"

int main(int argc, char *argv[argc + 1]) {
  int cl_fd;
  struct sockaddr_un peerAddr;
  ssize_t numRead;
  char buf[BUF_SIZE];

  if ((cl_fd = yunixAconnect(SV_SOCK_PATH, &peerAddr, SOCK_STREAM)) == -1)
    systmErr("yunixAconnect() failed");

  while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
    if (write(cl_fd, buf, numRead) != numRead)
      custmErr("partial/failed write()");

  if (numRead == -1)
    systmErr("read() failed");

  exit(EXIT_SUCCESS);
}

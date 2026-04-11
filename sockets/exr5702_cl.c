/* =========================================================================
 * Created on: <Wed Apr 08 15:42:44 +01 2026>
 * Time-stamp: <Sat Apr 11 04:59:58 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/yus_axfr_cl_ex_57_2.c -
 * Client for Exercise 57.2 Rewrite the programs in Listing 57-3
 * [[file:us_xfr_sv.c]] and Listing 57-4 [[file:us_xfr_cl.c]]
 * to use the Linux-specific abstract socket namespace (Section 57.6).
 * See peer at [[file:exr5702_sv.c]]
 * ========================================================================= */
#include "us_xfr.h"

int main(int argc, char *argv[argc + 1]) {
  int cl_fd;
  struct sockaddr_un peerAddr;
  ssize_t numRead;
  char buf[BUF_SIZE];

  if ((cl_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    systmErr("socket() failed");

  /* Construct remote address based abstract socket namespace */
  memset(&peerAddr, 0, sizeof(struct sockaddr_un));
  peerAddr.sun_family = AF_UNIX;
  strncpy(peerAddr.sun_path + 1, SV_SOCK_PATH, sizeof(peerAddr.sun_path) - 2);

  if (connect(cl_fd, (struct sockaddr *)&peerAddr,
              sizeof(struct sockaddr_un)) == -1)
    systmErr("connect() failed");

  while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
    if (write(cl_fd, buf, numRead) != numRead)
      custmErr("partial/failed write()");

  if (numRead == -1)
    systmErr("read() failed");

  exit(EXIT_SUCCESS);
}

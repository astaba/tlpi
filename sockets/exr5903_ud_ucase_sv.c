/* =========================================================================
 * Created on: <Sun Apr 19 14:57:13 +01 2026>
 * Time-stamp: <Mon Apr 20 14:12:52 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/exr5903_ud_ucase_sv.c -
 *
 * Exercise 59.3 Apply [[file:yunix_sockets.c]] library to
 * [[file:ud_ucase_sv.c]]
 * ========================================================================= */
#include "ud_ucase.h"
#include "unix_sockets.h"

int main(int argc, char *argv[]) {
  struct sockaddr_un localaddr, claddr;
  int svfd, j;
  ssize_t numBytes;
  socklen_t len;
  char buf[BUF_SIZE];

  /* Create a UNIX SOCK_DGRAM socket fd and bind() to loocal sockaddr_un */
  svfd = yunixbind(SV_SOCK_PATH, &localaddr, SOCK_DGRAM);
  if (svfd == -1)
    systmErr("yunixbind() failed");

  /* Receive messages, convert to uppercase, and return to client */
  for (;;) {
    len = sizeof(struct sockaddr_un);
    numBytes =
        recvfrom(svfd, buf, BUF_SIZE, 0, (struct sockaddr *)&claddr, &len);
    if (numBytes == -1)
      systmErr("recvfrom");

    printf("Server received %ld bytes from %s\n", (long)numBytes,
           claddr.sun_path);

    for (j = 0; j < numBytes; j++)
      buf[j] = toupper((unsigned char)buf[j]);

    if (sendto(svfd, buf, numBytes, 0, (struct sockaddr *)&claddr, len) !=
        numBytes)
      custmErr("sendto");
  }
}

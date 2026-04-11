/* =========================================================================
 * Created on: <Wed Apr 08 20:58:08 +01 2026>
 * Time-stamp: <Fri Apr 10 08:40:14 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/exr5701_sv.c -
 * Server for Exercise 57.1: In Section 57.3, we noted that UNIX domain
 * datagram sockets are reliable.  Write programs to show that if a sender
 * transmits datagrams to a UNIX domain datagram socket faster than the
 * receiver reads them, then the sender is eventually blocked, and remains
 * blocked until the receiver reads some of the pending datagrams.
 * NOTE: In this scenario server plays the role of spamming sender.
 * See Client at [[file:exr5701_cl.c]]
 * Explanation: [[file:README.org::#exercise-57-1]]
 * ========================================================================= */
#include "ud_ucase.h"
#define BUFMAX 4096

int main(int argc, char *argv[argc + 1]) {
  int sv_fd;
  struct sockaddr_un sv_addr, cl_addr;
  socklen_t cl_addrlen;
  ssize_t numRecv, numSent;
  char buf[BUFMAX];
  size_t i;

  if ((sv_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    systmErr("socket() failed");

  /* Bind socket to a file-system entry to connectable system wide */
  memset(&sv_addr, 0, sizeof(struct sockaddr_un));
  sv_addr.sun_family = AF_UNIX;
  strncpy(sv_addr.sun_path + 1, SV_SOCK_PATH, sizeof(sv_addr.sun_path) - 2);

  if (bind(sv_fd, (struct sockaddr *)&sv_addr,
           (socklen_t)sizeof(struct sockaddr_un)) == -1)
    systmErr("bind() failed");

  cl_addrlen = sizeof(struct sockaddr_un);
  numRecv = recvfrom(sv_fd, buf, BUFMAX, 0, (struct sockaddr *)&cl_addr,
                     &cl_addrlen);
  if (numRecv == -1)
    systmErr("recvfrom() failed");
  printf("Server Received ← \"%.*s\"\n", (int)numRecv, buf);

  /* Now that you have the peer address, flood its Kernel reception buffer */
  for (i = 0; i < (1 << 10); i++) {
    snprintf(buf, BUFMAX, "%06zu:ABCDFEGHIJKLMNOPQRSTUVWXYZ", i);
    numSent = sendto(sv_fd, buf, strlen(buf), 0, (struct sockaddr *)&cl_addr,
                     cl_addrlen);
    if (numSent != (ssize_t)strlen(buf))
      custmErr("senfto() failed");
    printf("Server Sent Response → \"%s\"\n", buf);
  }
}

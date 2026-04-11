/* =========================================================================
 * Created on: <Wed Apr 08 19:56:45 +01 2026>
 * Time-stamp: <Thu Apr  9 11:44:13 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/exr5701_cl.c -
 * Client for Exercise 57.1: In Section 57.3, we noted that UNIX domain
 * datagram sockets are reliable.  Write programs to show that if a sender
 * transmits datagrams to a UNIX domain datagram socket faster than the
 * receiver reads them, then the sender is eventually blocked, and remains
 * blocked until the receiver reads some of the pending datagrams.
 * NOTE: In this scenario client plays the role of receiving bottleneck.
 * See Server at [[file:exr5701_sv.c]]
 * Explanation: [[file:README.org::#exercise-57-1]]
 * ========================================================================= */
#include "ud_ucase.h"

int main(int argc, char *argv[argc + 1]) {
  int cl_fd;
  struct sockaddr_un cl_addr, sv_addr;
  ssize_t numSent, numRecv;
  char *payload = "Sent me all the data.";
  char buf[4096];

  if ((cl_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    systmErr("sockst() failed");

  memset(&cl_addr, 0, sizeof(struct sockaddr));
  cl_addr.sun_family = AF_UNIX;
  snprintf(cl_addr.sun_path + 1, sizeof(cl_addr.sun_path) - 1,
           "yud_abstract.%ld", (long)getpid());

  if (bind(cl_fd, (struct sockaddr *)&cl_addr,
           (socklen_t)sizeof(struct sockaddr_un)) == -1)
    systmErr("bind() failed");

  memset(&sv_addr, 0, sizeof(struct sockaddr_un));
  sv_addr.sun_family = AF_UNIX;
  strncpy(sv_addr.sun_path + 1, SV_SOCK_PATH, sizeof(sv_addr.sun_path) - 2);

  /* Send the first datagram to advertise local address to peer server. */
  numSent =
      sendto(cl_fd, payload, strlen(payload), 0, (struct sockaddr *)&sv_addr,
             (socklen_t)sizeof(struct sockaddr_un));
  if (numSent != (ssize_t)strlen(payload))
    custmErr("sendto() failed");

  /* Sleep enough for the reception kernel buffer to be flooded by server. */
  sleep(30);

  /* Unclog reception queue */
  while ((numRecv = recvfrom(cl_fd, buf, 4096, 0, NULL, NULL)) > 0) {
    printf("Client Received Reponse ← \"%.*s\"\n", (int)numRecv, buf);
  }

  if (numRecv == -1)
    systmErr("recvfrom() failed");

  exit(EXIT_SUCCESS);
}

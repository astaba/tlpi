/* =========================================================================
 * Created on: <Wed Apr 08 19:56:45 +01 2026>
 * Time-stamp: <Mon Apr 20 16:49:22 +01 2026 by owner>
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
#include <sys/socket.h>

int main(int argc, char *argv[argc + 1]) {
  int cl_fd;
  char localpath[SUN_PATHSIZE];
  struct sockaddr_un cl_addr, sv_addr;
  socklen_t socklen;
  ssize_t numSent, numRecv;
  char *payload = "Sent me all the data.";
  char buf[4096];

  if ((cl_fd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    systmErr("sockst() failed");

  memset(&cl_addr, 0, sizeof(struct sockaddr));
  cl_addr.sun_family = AF_UNIX;
  snprintf(localpath, SUN_PATHSIZE, "yud_abstract.%ld", (long)getpid());
  strncpy(cl_addr.sun_path + 1, localpath, sizeof(cl_addr.sun_path) - 2);

  socklen = offsetof(struct sockaddr_un, sun_path) + 1 + strlen(localpath);
  if (bind(cl_fd, (struct sockaddr *)&cl_addr, socklen) == -1)
    systmErr("bind() failed");

  memset(&sv_addr, 0, sizeof(struct sockaddr_un));
  sv_addr.sun_family = AF_UNIX;
  strncpy(sv_addr.sun_path + 1, SV_SOCK_PATH, sizeof(sv_addr.sun_path) - 2);

  /* Send the first datagram to advertise local address to peer server. */
  socklen = offsetof(struct sockaddr_un, sun_path) + 1 + strlen(SV_SOCK_PATH);
  numSent = sendto(cl_fd, payload, strlen(payload), 0,
                   (struct sockaddr *)&sv_addr, socklen);

  /* SUN_LEN(&sv_addr)); WARN: EINVAL Invalid argument to bind() */
  /* NOTE: Never use SUN_LEN() on Path from the Abstract Namespace */

  if (numSent != (ssize_t)strlen(payload))
    systmErr("sendto() failed");

  /* Sleep enough for the reception kernel buffer to be flooded by server. */
  sleep(30);

  /* Unclog reception queue */
  while ((numRecv = recvfrom(cl_fd, buf, 4096, MSG_DONTWAIT, NULL, NULL)) > 0) {
    printf("Client Received Reponse ← \"%.*s\"\n", (int)numRecv, buf);
  }

  if (numRecv == -1) {
    if (errno == EAGAIN || errno == EWOULDBLOCK)
      custmErr("Unresponsive peer!");
    else
      systmErr("recvfrom() failed");
  }

  exit(EXIT_SUCCESS);
}

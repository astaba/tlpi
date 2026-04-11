/* =========================================================================
 * Created on: <Thu Apr 09 13:52:37 +01 2026> 
 * Time-stamp: <Fri Apr 10 14:24:12 +01 2026 by owner> 
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/exr5704_b.c -
 * Exercise 57.4 Test connected socket receiving third party's packets.
 * Run in order: This, then [[file:exr5704_a.c]] and [[file:exr5704_c.c]].
 * Explanation at [[file:README.org::#exercise-57-4]]
 * ========================================================================= */
#include "exr5704.h"
#define BACKLOG 5

int main(int argc, char *argv[argc + 1]) {
  int sfd, i;
  struct sockaddr_un localAddr, peerAddr;
  char buf[BUF_SIZE];
  socklen_t peerLen;
  ssize_t numRecv, numSent;

  if ((sfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    systmErr("B: socket() failed");

  memset(&localAddr, 0, sizeof(struct sockaddr_un));
  localAddr.sun_family = AF_UNIX;
  strncpy(localAddr.sun_path + 1, PATH_B, sizeof(localAddr.sun_path) - 2);

  if (bind(sfd, (struct sockaddr *)&localAddr,
           (socklen_t)sizeof(struct sockaddr_un)) == -1)
    systmErr("B: bind() failed");

  /* WARN: UNIX DATAGRAMS Sockets don't call listen() */
  /* UNIX Datagrams Transaction loop */
  while (1) {
    peerLen = sizeof(struct sockaddr_un);
    numRecv = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *)&peerAddr,
                       &peerLen);
    if(numRecv == -1)
      systmErr("B: recvfrom() failed");

    printf("B ←-- %s: \"%.*s\"\n", peerAddr.sun_path, (int)numRecv, buf);
    for (i = 0; i < numRecv; i++)
      buf[i] = toupper((unsigned char)buf[i]);

    numSent =
        sendto(sfd, buf, numRecv, 0, (struct sockaddr *)&peerAddr, peerLen);
    if(numSent != numRecv)
      custmErr("B: senfto() failed");
    printf("B --→ \"%.*s\"\n", (int)numSent, buf);
  }
}

/* =========================================================================
 * Created on: <Sat Apr 11 21:52:21 +01 2026>
 * Time-stamp: <Sat Apr 11 23:51:25 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/yi6d_ucase_sv.c -
 * See [[file:i6d_ucase_sv.c]], [[file:yi6d_ucase_cl.c]]
 * ========================================================================= */
#include "i6d_ucase.h"

int main(int argc, char *argv[argc + 1]) {
  int sfd, i;
  struct sockaddr_in6 locali6, peeri6;
  socklen_t peerLen;
  ssize_t numRecv;
  char buf[BUF_SIZE], peerStr[INET6_ADDRSTRLEN];

  if ((sfd = socket(PF_INET6, SOCK_DGRAM, 0)) == -1)
    systmErr("socket() failed");

  memset(&locali6, 0, sizeof(struct sockaddr_in6));
  locali6.sin6_family = AF_INET6;
  locali6.sin6_port = htons(PORT_NUM);
  locali6.sin6_addr = in6addr_any;

  if (bind(sfd, (struct sockaddr *)&locali6, sizeof(struct sockaddr_in6)) == -1)
    systmErr("bind() failed");

  for (;;) {
    peerLen = sizeof(struct sockaddr_in6);
    numRecv =
      recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *)&peeri6, &peerLen);
    if (numRecv == -1)
      systmErr("recvfrom() failed");

    /* WARN: Pass nested pointer to nested struct with Unique field */
    /* ... t_ntop(AF_INET6, FIXME:&peeri6, peerStr, INET6_ADDRS ...  */
    /* HACK: This is the very memory address: &peeri6.sin6_addr.s6_addr */
    /* &peeri6.sin6_addr works only because sin6_addr has only one field. */
    if (inet_ntop(AF_INET6, &peeri6.sin6_addr, peerStr, INET6_ADDRSTRLEN) == NULL)
      systmErr("inet_ntop() failed");
    printf("Server ←─ %8ld bytes from (%s, %d)\n", (long)numRecv, peerStr,
           ntohs(peeri6.sin6_port));

    for (i = 0; i < numRecv; i++)
      buf[i] = toupper((unsigned int)buf[i]);

    if (sendto(sfd, buf, numRecv, 0, (struct sockaddr *)&peeri6, peerLen) !=
        numRecv)
      custmErr("sendto() failed");
  }
}

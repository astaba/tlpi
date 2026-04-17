/* =========================================================================
 * Created on: <Sat Apr 11 21:06:29 +01 2026>
 * Time-stamp: <Sat Apr 11 23:21:01 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/yi6d_ucase_cl.c -
 * See [[file:i6d_ucase_cl.c]], [[file:yi6d_ucase_sv.c]]
 * ========================================================================= */
#include "i6d_ucase.h"

int main(int argc, char *argv[argc + 1]) {
  int sfd, rc, i;
  struct sockaddr_in6 peeri6;
  size_t msgLen;
  ssize_t numRecv;
  char buf[BUF_SIZE];

  if (argc < 3 || !strcmp(argv[1], "--help"))
    usageErr("%s host-name msg ...\n", argv[0]);

  if ((sfd = socket(PF_INET6, SOCK_DGRAM, 0)) == -1)
    systmErr("socket() failed");

  memset(&peeri6, 0, sizeof(struct sockaddr_in6));
  peeri6.sin6_family = AF_INET6;
  peeri6.sin6_port = htons(PORT_NUM);
  if ((rc = inet_pton(AF_INET6, argv[1], &peeri6.sin6_addr)) != 1) {
    if (rc == 0)
      errno = EINVAL;
    systmErr("inet_pton() failed");
  }

  for (i = 2; i < argc; i++) {
    msgLen = strlen(argv[i]);
    if (sendto(sfd, argv[i], msgLen, 0, (struct sockaddr *)&peeri6,
               sizeof(struct sockaddr_in6)) != (ssize_t)msgLen)
      custmErr("sendto() failed");

    numRecv = recvfrom(sfd, buf, BUF_SIZE, 0, NULL, NULL);
    if (numRecv == -1)
      systmErr("recvfrom() failed");

    printf("Client \t←─ \"%.*s\"\n", (int)numRecv, buf);
  }

  exit(EXIT_SUCCESS);
}

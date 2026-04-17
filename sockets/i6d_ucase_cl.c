/* =========================================================================
 * Created on: <Sat Apr 11 19:21:37 +01 2026>
 * Time-stamp: <Mon Apr 13 04:06:25 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026
 *             See file [[file:../COPYING.gpl-v3]] for details GNU License.
 * Desc      : ~/coding/c_prog/tlpi/sockets/i6d_ucase_cl.c -
 *
 * Listing 59.4 i6d_ucase_cl.c Client for i6d_ucase_sv.c: send each
 * command-line argument as a datagram to the server, and then display
 * the contents of the server's response datagram.
 * See also [[file:i6d_ucase_sv.c]]
 * ========================================================================= */
#include "i6d_ucase.h"

int main(int argc, char *argv[]) {
  struct sockaddr_in6 svaddr;
  int sfd, j;
  size_t msgLen;
  ssize_t numBytes;
  char resp[BUF_SIZE];

  if (argc < 3 || strcmp(argv[1], "--help") == 0)
    usageErr("%s host-address msg...\n", argv[0]);

  /* Create a datagram socket; send to an address in the IPv6 domain */

  sfd = socket(AF_INET6, SOCK_DGRAM, 0); /* Create client socket */
  if (sfd == -1)
    systmErr("socket");

  memset(&svaddr, 0, sizeof(struct sockaddr_in6));
  svaddr.sin6_family = AF_INET6;
  svaddr.sin6_port = htons(PORT_NUM);
  if (inet_pton(AF_INET6, argv[1], &svaddr.sin6_addr) <= 0)
    custmErr("inet_pton failed for address '%s'", argv[1]);

  /* Send messages to server; echo responses on stdout */

  for (j = 2; j < argc; j++) {
    msgLen = strlen(argv[j]);
    if (sendto(sfd, argv[j], msgLen, 0, (struct sockaddr *)&svaddr,
               sizeof(struct sockaddr_in6)) != (ssize_t)msgLen)
      custmErr("sendto");

    numBytes = recvfrom(sfd, resp, BUF_SIZE, 0, NULL, NULL);
    if (numBytes == -1)
      systmErr("recvfrom");

    printf("Response %d: %.*s\n", j - 1, (int)numBytes, resp);
  }

  exit(EXIT_SUCCESS);
}

/* =========================================================================
 * Created on: <Tue Apr 07 19:16:26 +01 2026>
 * Time-stamp: <Wed Apr  8 02:15:14 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026
 *             See file COPYING.gpl-v3 for details GNU License.
 * Desc      : ~/coding/c_prog/tlpi/sockets/ud_ucase_sv.c -
 * Listing 57.6 A server that uses a UNIX domain datagram socket
 * to receive datagrams, convert their contents to uppercase,
 * and then return them to the senders.
 * See also [[file:ud_ucase_cl.c]].
 * ========================================================================= */
#include "ud_ucase.h"

int main(int argc, char *argv[]) {
  struct sockaddr_un svaddr, claddr;
  int svfd, j;
  ssize_t numBytes;
  socklen_t len;
  char buf[BUF_SIZE];

  /* Create server socket */
  if ((svfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    systmErr("socket");

  /* Construct well-known address and bind server socket to it */
  /* HACK: Follow this link for explanation on both following checks:
     [[file:README.org::#stream-sockets-in-the-unix-domain]] */
  if (strlen(SV_SOCK_PATH) > sizeof(svaddr.sun_path) - 1)
    custmErr("Server socket path too long: %s", SV_SOCK_PATH);
  /* WARN: Make sure path didn't exist before being created anew. */
  if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT)
    systmErr("remove-%s", SV_SOCK_PATH);

  memset(&svaddr, 0, sizeof(struct sockaddr_un));
  svaddr.sun_family = AF_UNIX;
  strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

  if (bind(svfd, (struct sockaddr *)&svaddr, sizeof(struct sockaddr_un)) == -1)
    systmErr("bind");

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

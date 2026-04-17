/* =========================================================================
 * Created on: <Tue Apr 07 19:12:19 +01 2026>
 * Time-stamp: <Mon Apr 13 03:00:48 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026
 *             See file [[file:../COPYING.gpl-v3]] for details GNU License.
 * Desc      : ~/coding/c_prog/tlpi/sockets/ud_ucase_cl.c -
 * Listing 57.7 A UNIX domain client that communicates with the server
 * in [[file:ud_ucase_sv.c]]. This client sends each command-line argument
 * as a datagram to the server, and then displays the contents of the
 * server's response datagram.
 * ========================================================================= */
#include "ud_ucase.h"

int main(int argc, char *argv[]) {
  struct sockaddr_un svaddr, claddr;
  int clfd, j;
  size_t msgLen;
  ssize_t numBytes;
  char resp[BUF_SIZE];

  if (argc < 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s msg...\n", argv[0]);

  /* Create client socket; bind to unique pathname (based on PID) */
  if ((clfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    systmErr("socket");

  memset(&claddr, 0, sizeof(struct sockaddr_un));
  claddr.sun_family = AF_UNIX;
  snprintf(claddr.sun_path, sizeof(claddr.sun_path), "/tmp/ud_ucase_cl.%ld",
           (long)getpid());

  /* WARN: Contrary to Internet Domain, in the UNIX Domain the client
   * must bind() to create its own entry in the file system if it is
   * ever to receive a response. */
  if (bind(clfd, (struct sockaddr *)&claddr, sizeof(struct sockaddr_un)) == -1)
    systmErr("bind");

  /* Construct address of server */
  memset(&svaddr, 0, sizeof(struct sockaddr_un));
  svaddr.sun_family = AF_UNIX;
  strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

  /* Send messages to server; echo responses on stdout */
  for (j = 1; j < argc; j++) {
    msgLen = strlen(argv[j]); /* May be longer than BUF_SIZE */
    if (sendto(clfd, argv[j], msgLen, 0, (struct sockaddr *)&svaddr,
               sizeof(struct sockaddr_un)) != (ssize_t)msgLen)
      custmErr("sendto");

    numBytes = recvfrom(clfd, resp, BUF_SIZE, 0, NULL, NULL);
    /* Or equivalently: numBytes = recv(clfd, resp, BUF_SIZE, 0);
       or: numBytes = read(clfd, resp, BUF_SIZE); */
    if (numBytes == -1)
      systmErr("recvfrom");
    printf("Response %d: %.*s\n", j, (int)numBytes, resp);
  }

  remove(claddr.sun_path); /* Remove client socket pathname */
  exit(EXIT_SUCCESS);
}

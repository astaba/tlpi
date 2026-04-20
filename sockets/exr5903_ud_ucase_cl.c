/* =========================================================================
 * Created on: <Sun Apr 19 15:03:16 +01 2026>
 * Time-stamp: <Mon Apr 20 14:12:52 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/exr5903_ud_ucase_cl.c -
 *
 * Exercise 59.3 Apply [[file:yunix_sockets.c]] library to
 * [[file:ud_ucase_cl.c]]
 * ========================================================================= */
#include "ud_ucase.h"
#include "unix_sockets.h"

int main(int argc, char *argv[]) {
  struct sockaddr_un peeraddr, localaddr;
  char localpath[SUN_PATHSIZE];
  int clfd, j;
  size_t msgLen;
  ssize_t numBytes;
  char resp[BUF_SIZE];

  if (argc < 2 || strcmp(argv[1], "--help") == 0)
    usageErr("%s msg...\n", argv[0]);

  /* Build local path */
  snprintf(localpath, SUN_PATHSIZE, "/tmp/ud_ucase_cl.%ld", (long)getpid());

  /* Create a UNIX SOCK_DGRAM socket fd and bind() to local sockaddr_un */
  clfd = yunixbind(localpath, &localaddr, SOCK_DGRAM);
  if (clfd == -1)
    systmErr("yunixbind() failed");

  /* Build peer sockaddr_un */
  if (yunixaddress(SV_SOCK_PATH, &peeraddr))
    systmErr("yunixaddress() failed");

  /* Send messages to server; echo responses on stdout */
  for (j = 1; j < argc; j++) {
    msgLen = strlen(argv[j]); /* May be longer than BUF_SIZE */
    if (sendto(clfd, argv[j], msgLen, 0, (struct sockaddr *)&peeraddr,
               sizeof(struct sockaddr_un)) != (ssize_t)msgLen)
      custmErr("sendto");

    numBytes = recvfrom(clfd, resp, BUF_SIZE, 0, NULL, NULL);
    /* Or equivalently: numBytes = recv(clfd, resp, BUF_SIZE, 0);
       or: numBytes = read(clfd, resp, BUF_SIZE); */
    if (numBytes == -1)
      systmErr("recvfrom");
    printf("Response %d: %.*s\n", j, (int)numBytes, resp);
  }

  remove(localaddr.sun_path); /* Remove client socket pathname */
  exit(EXIT_SUCCESS);
}

/* =========================================================================
 * Created on: <Wed Apr 08 00:34:24 +01 2026>
 * Time-stamp: <Sat Apr 11 02:43:53 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/yud_ucase_cl.c -
 * Sandbox for [[file:ud_ucase_cl.c]]
 * See also [[file:yud_ucase_sv.c]]
 * ========================================================================= */
#include "ud_ucase.h"

int main(int argc, char *argv[argc + 1]) {
  int clfd, i;
  struct sockaddr_un claddr, svaddr;
  size_t msgLen;
  ssize_t numSent, numRead;
  char resbuf[BUF_SIZE];

  /* Validate command line arguments */
  if (argc < 2 || !strcmp(argv[1], "--help"))
    usageErr("%s msg...\n", argv[0]);

  /* (1) Create socket file descriptor for the client */
  if ((clfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    systmErr("socket() failed");

  /* (2) WARN: Exceptional to UNIX Domain, the client must create an
   * entry in the file-system by binding its socket to a file path. */

  /* (2.1) Construct struct sockaddr_un for the local address */
  memset(&claddr, 0, sizeof(struct sockaddr_un));
  claddr.sun_family = AF_UNIX;
  snprintf(claddr.sun_path, sizeof(claddr.sun_path), "/tmp/ud_ucase_cl.%ld",
           (long)getpid());

  /* (2.2) Bind client address to local address. */
  if (bind(clfd, (struct sockaddr *)&claddr, SUN_LEN(&claddr)) == -1)
    systmErr("bind() failed");

  /* (3) Construct struct sockaddr_un for the remote address */
  memset(&svaddr, 0, sizeof(struct sockaddr_un));
  svaddr.sun_family = AF_UNIX;
  strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

  /* (4) Iteratively send cmdline messages and feedback server response. */
  for (i = 1; i < argc; i++) {
    msgLen = strlen(argv[i]);
    numSent = sendto(clfd, argv[i], msgLen, 0, (struct sockaddr *)&svaddr,
                     SUN_LEN(&svaddr));
    if (numSent != (ssize_t)msgLen)
      custmErr("sendto() failed");

    numRead = recvfrom(clfd, resbuf, BUF_SIZE, 0, NULL, NULL);
    /* numRead = recv(clfd, resbuf, BUF_SIZE, 0); */
    /* numRead = read(clfd, resbuf, BUF_SIZE); */
    if (numRead == -1)
      systmErr("recvfrom() failed");
    /* Print response to stdout */
    printf("Response %d: '%.*s'\n", i, (int)numRead, resbuf);
  }

  /* (5) Unlink client path from file-system. */
  remove(claddr.sun_path);
  exit(EXIT_SUCCESS);
}

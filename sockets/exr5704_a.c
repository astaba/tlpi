/* =========================================================================
 * Created on: <Thu Apr 09 11:41:06 +01 2026>
 * Time-stamp: <Thu Apr  9 23:41:03 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/exr5704_a.c -
 * Exercise 57.4 Test connected socket receiving third party's packets.
 * Run in order: [[file:exr5704_b.c]] then This and [[file:exr5704_c.c]].
 * Explanation at [[file:README.org::#exercise-57-4]]
 * ========================================================================= */
#include "exr5704.h"

int main(int argc, char *argv[argc + 1]) {
  int sfd;
  struct sockaddr_un localAddr, peerAddr, anyAddr;
  socklen_t peerLen, anyLen;
  char buf[BUF_SIZE];
  ssize_t numRead, numRecv;

  if ((sfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    systmErr("A: socket() failed");

  memset(&localAddr, 0, sizeof(struct sockaddr_un));
  localAddr.sun_family = AF_UNIX;
  snprintf(localAddr.sun_path, sizeof(localAddr.sun_path), "%s", PATH_A);

  if (bind(sfd, (struct sockaddr *)&localAddr,
           (socklen_t)sizeof(struct sockaddr_un)) == -1)
    systmErr("A: bind() failed");

  memset(&anyAddr, 0, sizeof(struct sockaddr_un));
  /* HACK: Pass it to connect() to disconnect. */
  anyAddr.sun_family = AF_UNSPEC;

  memset(&peerAddr, 0, sizeof(struct sockaddr_un));
  peerAddr.sun_family = AF_UNIX;
  strncpy(peerAddr.sun_path + 1, PATH_B, sizeof(peerAddr.sun_path) - 2);

  /* Transaction loop */
  while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0 && *buf != '\n') {
    buf[strcspn(buf, "\n")] = '\0';

    if (!strncmp(buf, "c:", 2)) {
      /* Disconnect A to recvfrom() any no sendto() */
      anyLen = sizeof(struct sockaddr_un);
      if (connect(sfd, (struct sockaddr *)&anyAddr, anyLen) == -1)
        systmErr("A: connect(AF_UNSPEC) failed");

      numRecv = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *)&anyAddr,
                         &anyLen);
      if (numRecv == -1)
        systmErr("A ← 0: recvfrom() failed ");
      buf[numRecv] = 0;
      printf("A ← %s: \"%s\"\n", anyAddr.sun_path, buf);
    } else {
      /* Connect A to B */
      peerLen = sizeof(struct sockaddr_un);
      if (connect(sfd, (struct sockaddr *)&peerAddr, peerLen) == -1)
        systmErr("A: connect() failed");

      if (write(sfd, buf, numRead) != numRead)
        custmErr("A: partial/failed write()");

      if ((numRecv = recv(sfd, buf, BUF_SIZE, 0)) < 0)
        systmErr("recv() failed");
      printf(" A ← \"%.*s\"\n", (int)numRecv, buf);
    }
  }
  if (numRead == -1)
    systmErr("A: read() failed");

  /* Cleanup */
  remove(localAddr.sun_path);
  exit(EXIT_SUCCESS);
}

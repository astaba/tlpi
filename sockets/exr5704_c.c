/* =========================================================================
 * Created on: <Thu Apr 09 14:58:41 +01 2026>
 * Time-stamp: <Fri Apr 10 00:13:19 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/exr5704_c.c -
 * Exercise 57.4 Test connected socket receiving third party's packets.
 * Run in order: [[file:exr5704_b.c]] then [[file:exr5704_a.c]] and This.
 * Explanation at [[file:README.org::#exercise-57-4]]
 * ========================================================================= */
#include "exr5704.h"

int main(int argc, char *argv[argc + 1]) {
  int sfd;
  struct sockaddr_un localAddr, aAddr, peerAddr;
  char buf[BUF_SIZE], *p;
  ssize_t numRead, numRecv;
  size_t msgLen;

  if ((sfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    systmErr("C: socket() failed");

  memset(&localAddr, 0, sizeof(struct sockaddr_un));
  localAddr.sun_family = AF_UNIX;
  snprintf(localAddr.sun_path, sizeof(localAddr.sun_path), "%s.%ld", PATH_C,
           (long)getpid());

  if (bind(sfd, (struct sockaddr *)&localAddr,
           (socklen_t)sizeof(struct sockaddr_un)) == -1)
    systmErr("C: bind() failed");

  memset(&peerAddr, 0, sizeof(struct sockaddr_un));
  peerAddr.sun_family = AF_UNIX;
  strncpy(peerAddr.sun_path + 1, PATH_B, sizeof(peerAddr.sun_path) - 2);

  memset(&aAddr, 0, sizeof(struct sockaddr_un));
  aAddr.sun_family = AF_UNIX;
  strncpy(aAddr.sun_path, PATH_A, sizeof(aAddr.sun_path) - 1);

  /* Transaction loop */
  while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0 && *buf != '\n') {
    buf[strcspn(buf, "\n")] = '\0';
    if (!strncmp(buf, "a:", 2)) {
      /* On channel (C → A) only care about sending: no recvfrom() call */
      p = buf + strspn(buf, "a:");
      msgLen = strlen(p);
      if (sendto(sfd, p, msgLen, 0, (struct sockaddr *)&aAddr,
                 sizeof(struct sockaddr_un)) != (ssize_t)msgLen) {
        if (errno == EPERM)
          systmWrn("C → A: sendto() failed");
        else
          systmErr("C → A: sendto() failed");
      } else
	printf("C --→ %s: \"%s\"\n", aAddr.sun_path, p);
      continue;
    }
    /* On channel (C ←→ B) */
    msgLen = strlen(buf);
    if (sendto(sfd, buf, msgLen, 0, (struct sockaddr *)&peerAddr,
               sizeof(struct sockaddr_un)) != (ssize_t)msgLen)
      custmErr("C → B: partial/failed sendto()");

    numRecv = recvfrom(sfd, buf, BUF_SIZE, 0, NULL, NULL);
    if (numRecv == -1)
      systmErr("C: recvfrom() failed");
    printf("C ←-- :\"%.*s\"\n", (int)numRecv, buf);
  }
  if (numRead == -1)
    systmErr("C: read() failed");

  /* Cleanup */
  remove(localAddr.sun_path);
  exit(EXIT_SUCCESS);
}

/* =========================================================================
 * Created on: <Wed Apr 08 01:24:33 +01 2026>
 * Time-stamp: <Wed Apr  8 14:51:17 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/yud_ucase_sv.c -
 * Sandbox for [[file:ud_ucase_sv.c]]
 * See also [[file:yud_ucase_cl.c]]
 * ========================================================================= */
#include "ud_ucase.h"

int main(int argc, char *argv[argc + 1]) {
  int svfd, i;
  struct sockaddr_un svaddr, claddr;
  ssize_t numRecv, numSent;
  socklen_t addrlen;
  char buf[BUF_SIZE];

  /* (1) Create socket file descriptor for the server */
  if ((svfd = socket(AF_UNIX, SOCK_DGRAM, 0)) == -1)
    systmErr("socket() failed");

  /* (2.1) Make sure the entry in the file-system is created anew. */
  if (strlen(SV_SOCK_PATH) > sizeof(svaddr.sun_path) - 1)
    custmErr("Server socket path too long: %s", SV_SOCK_PATH);
  if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT)
    systmErr("remove(\"%s\") failed", SV_SOCK_PATH);

  /* (2.2) Construct struct sockaddr_un for the local address. */
  memset(&svaddr, 0, sizeof(struct sockaddr_un));
  svaddr.sun_family = AF_UNIX;
  strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

  /* (2.3) Bind server socket to local address */
  if (bind(svfd, (struct sockaddr *)&svaddr, sizeof(struct sockaddr_un)) == -1)
    systmErr("bind() failed");

  /* (4) Run infinite loop of iterative server. */
  while (1) {
    /* (4.1) WARN: init client's addrlen on each iteration and
     * retrieve its address from recvfrom() to be able to respond abck
     * to the specific client. */
    addrlen = sizeof(struct sockaddr_un);
    numRecv =
        recvfrom(svfd, buf, BUF_SIZE, 0, (struct sockaddr *)&claddr, &addrlen);
    if (numRecv == -1)
      systmErr("recvfrom() failed");

    /* (4.2) Feedback received data to local output */
    printf("Server received %ld bytes (%.*s) from: %s\n", (long)numRecv, (int)numRecv,
           buf, claddr.sun_path);

    /* (4.3) Build response */
    for (i = 0; i < numRecv; i++)
      buf[i] = toupper((unsigned int)buf[i]);

    /* (4.4) Send response back to iteration client. */
    numSent =
        sendto(svfd, buf, numRecv, 0, (struct sockaddr *)&claddr, addrlen);
    if (numSent != numRecv)
      custmErr("sendto() failed");
  }
}

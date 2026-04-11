/* =========================================================================
 * Created on: <Tue Apr 07 17:03:53 +01 2026>
 * Time-stamp: <Sat Apr 11 04:59:58 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/yus_xfr_sv.c -
 * Sandbox for [[file:us_xfr_sv.c]]
 * See client at [[file:yus_xfr_cl.c]]
 * ========================================================================= */
#include "us_xfr.h"
#define BACKLOG 5

/* Create a server bound to a stream socket in the UNIX domain */
int main(int argc, char *argv[argc + 1]) {
  int lisnfd, connfd;
  ssize_t numRead;
  char buf[BUFSIZ];
  struct sockaddr_un localAddr;

  /* (1) Created a socket file descriptor for the server */
  if ((lisnfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    systmErr("socket() failed");

  /* (2.1) Remove any existing path identic to future socket path */
  /* WARN: bind() needs properly populated struct sockaddr, but make sure
   * to check path validity BEFORE populating the address. */
  /* Before clearing file-system entry make sure to clear the right one */
  if (strlen(SV_SOCK_PATH) > (sizeof(localAddr.sun_path) - 1))
    custmErr("Server socket path too long: %s", SV_SOCK_PATH);
  /* Before populating address make sure the path did not exist. */
  if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT)
    systmErr("remove(\"%s\") failed", SV_SOCK_PATH);

  /* (2.2) Construct struct sockaddr_un for the local address */
  memset(&localAddr, 0, sizeof(struct sockaddr_un));
  localAddr.sun_family = AF_UNIX;
  strncpy(localAddr.sun_path, SV_SOCK_PATH, sizeof(localAddr.sun_path) - 1);

  /* (2.3) Bind server socket to local address */
  if (bind(lisnfd, (struct sockaddr *)&localAddr, SUN_LEN(&localAddr)) == -1)
    systmErr("bind() failed");

  /* (3) Make the socket passive for listening */
  if (listen(lisnfd, BACKLOG) == -1)
    systmErr("listen() failed");

  /* (4) Run iterative server infinite loop. */
  while (1) {
    /* (4.1) Wait on client and return a connection file descriptor */
    if ((connfd = accept(lisnfd, NULL, NULL)) == -1)
      systmErr("accept() failed");

    /* (4.2) Manage TCP I/O */
    while ((numRead = read(connfd, buf, BUFSIZ)) > 0)
      if (write(STDOUT_FILENO, buf, numRead) != numRead)
        custmErr("partial/failed write()");

    if (numRead == -1)
      systmErr("read() failed");

    /* (4.3) Drop client. */
    if (close(connfd) == -1)
      systmErr("close() failed");
  }
  /* Terminate server with manual signal. */
}

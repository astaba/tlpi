/* =========================================================================
 * Created on: <Tue Apr 07 02:14:29 +01 2026>
 * Time-stamp: <Tue Apr  7 15:24:30 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/us_socket.c -
 * Listing 57.1: Binding a UNIX domain socket
 * ========================================================================= */
#include "../lib/tlpi_hdr.h"	/* IWYU pragma: keep */
#include <sys/socket.h>
#include <sys/un.h>

void SANBOX(void) {

  char *SOCKNAME = "/tmp/socket_95123";
  int sockfd;
  struct sockaddr_un addr;
  socklen_t addrlen;

  /* Create socket */
  if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    systmErr("socket() failed");

  addrlen = sizeof(struct sockaddr_un);
  /* WARN: Always CLEAR address struct before use. */
  memset(&addr, 0, addrlen);
  /* Populate sockaddr_un fields */
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKNAME, strlen(SOCKNAME) - 1);
  /* HACK: The "- 1" takes advantage of the memset() to ensure that
     socket path is null-terminated in the limit of its field size. */

  if (bind(sockfd, (struct sockaddr *)&addr, addrlen) == -1)
    systmErr("bind() failed");
}

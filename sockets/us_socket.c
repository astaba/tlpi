/* =========================================================================
 * Created on: <Tue Apr 07 02:14:29 +01 2026>
 * Time-stamp: <Sat Apr 11 04:56:44 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/us_socket.c -
 * Listing 57.1: Binding a UNIX domain socket
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include <sys/socket.h>
#include <sys/un.h>

void SANBOX(void) {

  char *SOCKNAME = "/tmp/socket_95123";
  int sockfd;
  struct sockaddr_un addr;
  size_t addrSize;

  /* Create socket */
  if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
    systmErr("socket() failed");

  /* WARN: Always CLEAR address struct before use. */
  memset(&addr, 0, sizeof(struct sockaddr_un));
  /* Populate sockaddr_un fields */
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKNAME, strlen(SOCKNAME) - 1);
  /* HACK: The "- 1" takes advantage of the memset() to ensure that
     socket path is null-terminated in the limit of its field size. */

  /* addrSize = (offsetof(struct sockaddr_un, sun_path) + strlen(addr.sun_path)); */
  /* if (bind(sockfd, (struct sockaddr *)&addr, addrSize) == -1) */

  if (bind(sockfd, (struct sockaddr *)&addr, SUN_LEN(&addr)) == -1)
    systmErr("bind() failed");
}

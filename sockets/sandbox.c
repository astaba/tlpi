/* =========================================================================
 * Created on: <Fri Apr 10 21:18:01 +01 2026> 
 * Time-stamp: <Sat Apr 11 02:02:52 +01 2026 by owner> 
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/sandbox.c - 
 * ========================================================================= */
#include <netinet/in.h>
#include <string.h>

int main(int argc, char *argv[argc + 1]) {

  struct sockaddr_storage *storage;
  struct sockaddr_in6 addr;

  memset(&addr,0, sizeof(struct sockaddr_in6));

  addr.sin6_family = AF_INET6;
  /* addr.sin6_addr = IN6ADDR_ANY_INIT; */
  addr.sin6_addr = in6addr_any;
  addr.sin6_port = htons(8080);

  storage = (struct sockaddr_storage*)&addr;
}

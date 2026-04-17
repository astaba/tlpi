/* =========================================================================
 * Created on: <Wed Apr 15 13:04:07 +01 2026>
 * Time-stamp: <Wed Apr 15 14:27:43 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026
 *             See file [[file:../COPYING.gpl-v3]] for details GNU License.
 * Desc      : ~/coding/c_prog/tlpi/sockets/inet_sockets.c -
 *
 * Listing 59.9 A package of useful functions for Internet domain sockets.
 *
 * The following arguments are common to several of the functions below:
 *	'host'   :	NULL for loopback IP address, or a host name or
 *	                presentation-format IP address.
 *	'service':      either a name or a decimal port number string.
 *	'type'   :      either SOCK_STREAM or SOCK_DGRAM.
 * ========================================================================= */
#define _GNU_SOURCE       /* To get NI_MAXHOST and NI_MAXSERV                  \
                             definitions from <netdb.h> */
#include "inet_sockets.h" /* Declares functions defined here */
#include "../lib/tlpi_hdr.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

/* Create socket and connect it to the address specified by
  'host' + 'service'/'type'. Return socket descriptor on success,
  or -1 on error */
int inetConnect(const char *host, const char *service, int type) {
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int sfd, s;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;
  hints.ai_family = AF_UNSPEC; /* Allows IPv4 or IPv6 */
  hints.ai_socktype = type;

  s = getaddrinfo(host, service, &hints, &result);
  if (s != 0) {
    errno = ENOSYS;
    return -1;
  }

  /* Walk through returned list until we find an address structure
     that can be used to successfully connect a socket */

  for (rp = result; rp != NULL; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1)
      continue; /* On error, try next address */

    if (connect(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
      break; /* Success */

    /* Connect failed: close this socket and try next address */

    close(sfd);
  }

  freeaddrinfo(result);

  return (rp == NULL) ? -1 : sfd;
}

/* Create an Internet domain socket and bind it to the address
   { wildcard-IP-address + 'service'/'type' }.
   If 'doListen' is TRUE, then make this a listening socket (by
   calling listen() with 'backlog'), with the SO_REUSEADDR option set.
   If 'addrLen' is not NULL, then use it to return the size of the
   address structure for the address family for this socket.
   Return the socket descriptor on success, or -1 on error. */

static int /* Public interfaces: inetBind() and inetListen() */
inetPassiveSocket(const char *service, int type, socklen_t *addrlen,
                  Boolean doListen, int backlog) {
  struct addrinfo hints;
  struct addrinfo *result, *rp;
  int sfd, optval, s;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;
  hints.ai_socktype = type;
  hints.ai_family = AF_UNSPEC; /* Allows IPv4 or IPv6 */
  hints.ai_flags = AI_PASSIVE; /* Use wildcard IP address */

  s = getaddrinfo(NULL, service, &hints, &result);
  if (s != 0)
    return -1;

  /* Walk through returned list until we find an address structure
     that can be used to successfully create and bind a socket */

  optval = 1;
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1)
      continue; /* On error, try next address */

    if (doListen) {
      if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) ==
          -1) {
        close(sfd);
        freeaddrinfo(result);
        return -1;
      }
    }

    if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
      break; /* Success */

    /* bind() failed: close this socket and try next address */

    close(sfd);
  }

  if (rp != NULL && doListen) {
    if (listen(sfd, backlog) == -1) {
      freeaddrinfo(result);
      return -1;
    }
  }

  if (rp != NULL && addrlen != NULL)
    *addrlen = rp->ai_addrlen; /* Return address structure size */

  freeaddrinfo(result);

  return (rp == NULL) ? -1 : sfd;
}

/* Create stream socket, bound to wildcard IP address + port given in
  'service'. Make the socket a listening socket, with the specified
  'backlog'. Return socket descriptor on success, or -1 on error. */

int inetListen(const char *service, int backlog, socklen_t *addrlen) {
  return inetPassiveSocket(service, SOCK_STREAM, addrlen, TRUE, backlog);
}

/* Create socket bound to wildcard IP address + port given in
   'service'. Return socket descriptor on success, or -1 on error. */

int inetBind(const char *service, int type, socklen_t *addrlen) {
  return inetPassiveSocket(service, type, addrlen, FALSE, 0);
}

/**
 * Given a socket address in 'addr', whose length is specified in
 * 'addrlen', return a null-terminated string containing the host and
 * service names in the form "(hostname, port#)". The string is
 * returned in the buffer pointed to by 'addrStr', and this value is
 * also returned as the function result. The caller must specify the
 * size of the 'addrStr' buffer in 'addrStrLen'.
 *
 * @param addr: address of read-only memory place holding struct
 *              sockaddr to build internet address string from.
 * @param addrlen: exact size of specific inet sockaddr struct.
 * @param addrStr: address of memory place to output result string.
 * @param addrStrLen: size of the output memory place.
 */
char *inetAddressStr(const struct sockaddr *addr, socklen_t addrlen,
                     char *addrStr, int addrStrLen) {
  char host[NI_MAXHOST], service[NI_MAXSERV];

  if (getnameinfo(addr, addrlen, host, NI_MAXHOST, service, NI_MAXSERV,
                  NI_NUMERICSERV) == 0)
    snprintf(addrStr, addrStrLen, "(%s, %s)", host, service);
  else
    snprintf(addrStr, addrStrLen, "(?UNKNOWN?)");

  return addrStr;
}

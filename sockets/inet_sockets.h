/* =========================================================================
 * Created on: <Wed Apr 15 13:12:56 +01 2026>
 * Time-stamp: <Wed Apr 15 13:15:05 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026
 *             See file [[file:../COPYING.gpl-v3]] for details GNU License.
 * Desc      : ~/coding/c_prog/tlpi/sockets/inet_sockets.h -
 *
 * Listing 59.8 Header file for inet_sockets.c.
 * ========================================================================= */
#ifndef INET_SOCKETS_H
#define INET_SOCKETS_H /* Prevent accidental double inclusion */

#include <netdb.h>
#include <sys/socket.h>

int inetConnect(const char *host, const char *service, int type);

int inetListen(const char *service, int backlog, socklen_t *addrlen);

int inetBind(const char *service, int type, socklen_t *addrlen);

char *inetAddressStr(const struct sockaddr *addr, socklen_t addrlen,
                     char *addrStr, int addrStrLen);

#define IS_ADDR_STR_LEN 4096
/* Suggested length for string buffer that caller
   should pass to inetAddressStr(). Must be greater
   than (NI_MAXHOST + NI_MAXSERV + 4) */

#endif	/* INET_SOCKETS_H */

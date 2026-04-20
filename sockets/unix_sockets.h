/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2026.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU Lesser General Public License as published   *
* by the Free Software Foundation, either version 3 or (at your option)   *
* any later version. This program is distributed without any warranty.    *
* See the files COPYING.lgpl-v3 and [[file:../COPYING.gpl-v3]] for details. *
\*************************************************************************/

/* Solution for Exercise 59.3:a */

/* unix_sockets.h

   Header file for unix_sockets.c.
*/
#ifndef UNIX_SOCKETS_H
#define UNIX_SOCKETS_H /* Prevent accidental double inclusion */

#define SUN_PATHSIZE 92 /* sockaddr_un portable path size */

#include <stdbool.h>
#include <sys/socket.h>
#include <sys/un.h>

int unixBuildAddress(const char *path, struct sockaddr_un *addr);
int unixConnect(const char *path, int type);
int unixBind(const char *path, int type);

/* My own lib */
int yunixaddress(const char *path, struct sockaddr_un *addr);
int yunixconnect(const char *path, struct sockaddr_un *addr, int type);
int yunixbind(const char *path, struct sockaddr_un *addr, int type);
int yunixAaddress(const char *path, struct sockaddr_un *addr);
int yunixAconnect(const char *path, struct sockaddr_un *addr, int type);
int yunixAbind(const char *path, struct sockaddr_un *addr, int type);


#endif

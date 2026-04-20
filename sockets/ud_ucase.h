/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2026.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file [[file:../COPYING.gpl-v3]] for details. *
\*************************************************************************/

/* Listing 57.5 */

/* ud_ucase.h

   Header file for ud_ucase_sv.c and ud_ucase_cl.c.

   These programs employ sockets in /tmp. This makes it easy to compile
   and run the programs. However, for a security reasons, a real-world
   application should never create sensitive files in /tmp. (As a simple of
   example of the kind of security problems that can result, a malicious
   user could create a file using the name defined in SV_SOCK_PATH, and
   thereby cause a denial of service attack against this application.
   See Section 38.7 of "The Linux Programming Interface" for more details
   on this subject.)
*/
#ifndef UD_UCASE_H
#define UD_UCASE_H

#include "../lib/tlpi_hdr.h"
#include <ctype.h>
#include <sys/socket.h>
#include <sys/un.h>

#define BUF_SIZE                                                               \
  10                    /* Maximum size of messages exchanged                  \
                           between client and server */
#define SUN_PATHSIZE 92 /* sockaddr_un portable path size */
#define SV_SOCK_PATH "/tmp/ud_ucase"

#endif /* UD_UCASE_H */

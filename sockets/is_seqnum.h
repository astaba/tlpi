/* =========================================================================
 * Created on: <Mon Apr 13 02:30:26 +01 2026>
 * Time-stamp: <Mon Apr 13 04:06:25 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026
 *             See file [[file:../COPYING.gpl-v3]] for details GNU License.
 * Desc      : ~/coding/c_prog/tlpi/sockets/is_seqnum.h -
 * Listing 59.5 Header file for is_seqnum_sv.c and is_seqnum_cl.c.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h"
#include "read_line.h" /* Declaration of readLine() */
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>

#define PORT_NUM "50000" /* Port number for server */

/* Size of string able to hold largest integer (including terminating '\n') */
#define INT_LEN 30

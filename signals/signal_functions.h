/* =========================================================================
 * Created on: <Thu May 14 17:03:15 +01 2026> 
 * Time-stamp: <Thu May 14 17:04:08 +01 2026 by owner> 
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file COPYING.gpl-v3 for details.
 * Desc      : ~/coding/c_prog/tlpi/signals/signal_functions.h -
 *
 * Header file for Listing 20-4 Header file for signal_functions.c.
 * ========================================================================= */
#ifndef SIGNAL_FUNCTIONS_H
#define SIGNAL_FUNCTIONS_H

#include <signal.h>
#include "../lib/tlpi_hdr.h"

int printSigMask(FILE *of, const char *msg);

int printPendingSigs(FILE *of, const char *msg);

void printSigset(FILE *of, const char *ldr, const sigset_t *mask);

#endif

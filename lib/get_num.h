/* =========================================================================
 * Created on: <Mon Apr 13 18:55:58 +01 2026>
 * Time-stamp: <Wed Apr 15 06:26:35 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026
 *             See file [[file:../COPYING.gpl-v3]] for details GNU License.
 * Desc      : ~/coding/c_prog/tlpi/lib/get_num.h -
 *
 * Listing 3.5 Header file for get_num.c.
 *
 * WARN: This 012 is Octal_12 not decimal_12: 012 == 10
 * NOTE: The use of Octal number has the great advantages of making
 * “visible” set binary bits: while keeping null all other octal
 * digits just increment EVENLY the target octal digit to set the
 * binary bit identified by offset from all the zeros in the
 * completely nullified least significant bits.
 * ========================================================================= */
#ifndef GET_NUM_H
#define GET_NUM_H

typedef enum { TGN_SUCCESS = 0, TGN_FAILURE = -1 } tgn_t;

#define GN_NONNEG 01 /* Value must be >= 0 */
#define GN_GT_0 02   /* Value must be > 0 */

#define TGN_IGN_TRAILGB 010
#define TGN_IGN_TRAILWS 020

/* By default, integers are decimal */
#define GN_ANY_BASE 0100 /* Can use any base - like strtol(3) */
#define GN_BASE_8 0200   /* Value is expressed in octal */
#define GN_BASE_16 0400  /* Value is expressed in hexadecimal */

long long getLLong(const char *arg, int flags, const char *name);
long getLong(const char *arg, int flags, const char *name);
int getInt(const char *arg, int flags, const char *name);

tgn_t tryGetLLong(const char *arg, int flags, const char *name,
                           long long *out);
tgn_t tryGetLong(const char *arg, int flags, const char *name,
                          long *out);
tgn_t tryGetInt(const char *arg, int flags, const char *name,
                         int *out);

#endif

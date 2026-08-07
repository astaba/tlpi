/* =========================================================================
 * Created on: <Mon Apr 13 18:55:58 +01 2026>
 * Time-stamp: <Sat Jul 18 15:04:23 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.lgpl-v3]] and
 *             [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/lib/tlpi_hdr.h -
 *
 * Listing 3-1
 * ========================================================================= */
/*
   Standard header file used by nearly all of our example programs.
*/
#ifndef TLPI_HDR_H
#define TLPI_HDR_H /* Prevent accidental double inclusion */

#include <stdio.h>     /* Standard I/O functions */
#include <stdlib.h>    /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <errno.h>     /* Declares errno and defines error constants */
#include <stdbool.h>   /* 'bool' type plus 'true' and 'false' constants */
#include <string.h>    /* Commonly used string-handling functions */
#include <sys/types.h> /* Type definitions used by many programs */
#include <unistd.h>    /* Prototypes for many system calls */

/* ========================================================================= */
/*
  Listing 3.5 Header file for get_num.c.

  WARN: This 012 is Octal_12 not decimal_12: 012 == 10
  NOTE: The use of Octal number has the great advantages of making
  “visible” set binary bits: while keeping null all other octal
  digits just increment EVENLY the target octal digit to set the
  binary bit identified by offset from all the zeros in the
  completely nullified least significant bits.
*/

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

tgn_t tryGetLLong(const char *arg, int flags, const char *name, long long *out);
tgn_t tryGetLong(const char *arg, int flags, const char *name, long *out);
tgn_t tryGetInt(const char *arg, int flags, const char *name, int *out);

tgn_t tryGetULLong(const char *arg, int flags, const char *name,
                   unsigned long long *out);
tgn_t tryGetULong(const char *arg, int flags, const char *name,
                  unsigned long *out);
tgn_t tryGetUInt(const char *arg, int flags, const char *name,
                 unsigned int *out);

#endif

/*
   Listing 3-2 Declares our error-handling functions
*/

#ifndef ERROR_FUNCTIONS_H
#define ERROR_FUNCTIONS_H

/* Error diagnostic routines */

void systmWrn(const char *format, ...);

#ifdef __GNUC__

/* This macro stops 'gcc -Wall' complaining that "control reaches
   end of non-void function" if we use the following functions to
   terminate main() or some other non-void function. */

#define NORETURN __attribute__((__noreturn__))
#else
#define NORETURN
#endif

void systmErr(const char *format, ...) NORETURN;
void _systmerr(const char *format, ...) NORETURN;
void nmsysErr(int errnum, const char *format, ...) NORETURN;
void custmErr(const char *format, ...) NORETURN;
void usageErr(const char *format, ...) NORETURN;
void cmdLineErr(const char *format, ...) NORETURN;

#endif
/* ========================================================================= */

/* Unfortunately some UNIX implementations define FALSE and TRUE -
   here we'll undefine them */

#ifdef TRUE
#undef TRUE
#endif

#ifdef FALSE
#undef FALSE
#endif

typedef enum { FALSE, TRUE } Boolean;

#define min(m, n) ((m) < (n) ? (m) : (n))
#define max(m, n) ((m) > (n) ? (m) : (n))

/* Some systems don't define 'socklen_t' */

#if defined(__sgi)
typedef int socklen_t;
#endif

#if defined(__sun)
#include <sys/file.h> /* Has definition of FASYNC */
#endif

#if !defined(O_ASYNC) && defined(FASYNC)
/* Some systems define FASYNC instead of O_ASYNC */
#define O_ASYNC FASYNC
#endif

#if defined(MAP_ANON) && !defined(MAP_ANONYMOUS)
/* BSD derivatives usually have MAP_ANON, not MAP_ANONYMOUS */
#define MAP_ANONYMOUS MAP_ANON

#endif

#if !defined(O_SYNC) && defined(O_FSYNC)
/* Some implementations have O_FSYNC instead of O_SYNC */
#define O_SYNC O_FSYNC
#endif

#if defined(__FreeBSD__)

/* FreeBSD uses these alternate names for fields in the sigval structure */

#define sival_int sigval_int
#define sival_ptr sigval_ptr
#endif

#endif

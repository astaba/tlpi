/* =========================================================================
 * Created on: <Mon Apr 13 20:37:24 +01 2026>
 * Time-stamp: <Wed Apr 15 06:26:36 +01 2026 by owner>
 * Author    : Astar Bahouidi
 * Desc      : ~/coding/c_prog/tlpi/lib/tryGet_num.c -
 *
 * Listing 3.6y Functions to process numeric command-line arguments.
 * =========================================================================
 * Library  : tryGet_num
 * Summary  : Non-terminating numeric conversion utilities for robust systems.
 * Logic    : Unlike the standard TLPI getInt() family, these functions
 * do not call exit(). They log errors to stderr and return a status
 * code (PARSE_SUCCESS/PARSE_FAILURE).
 *
 * Safety   :
 * 1. Intermediary 8-byte containers prevent stack-smashing during
 * down-casting (e.g., long long -> int).
 * 2. The "Exit Gate" pointer check enables "Type-Check Only" mode:
 * passing NULL as the 'out' pointer allows the caller to validate
 * string syntax without requiring storage for the result.
 *
 * Usage    : status = tryGetInt(argv[1], GN_GT_0, "port", &my_int);
 * ========================================================================= */
#include "get_num.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline int hasNonWhitespace(char *ptr) {
  for (; *ptr; ptr++) {
    if (!isspace((unsigned char)*ptr))
      return 1;
  }
  return 0;
}

/* Print a diagnostic message for non-fatal conversion errors.
   - fname: The calling function name (e.g., "tryGetInt")
   - msg:   A descriptive error message or result of strerror()
   - arg:   The raw string that failed conversion
   - name:  The logical name of the argument (e.g., "port-number") */
static void gnWarn(const char *fname, const char *msg, const char *arg,
                   const char *name) {
  fprintf(stderr, "%s error", fname);
  if (name != NULL)
    fprintf(stderr, " (in %s)", name);
  fprintf(stderr, ": %s\n", msg);
  if (arg != NULL && *arg != '\0')
    fprintf(stderr, "\toffending text: \"%s\"\n", arg);
}

/* Core numeric parser. Returns PARSE_SUCCESS (0) or PARSE_FAILURE (-1).
   Uses strtoll(3) for robust conversion. Validates empty strings,
   base-specific syntax, and non-numeric trailing characters. */
static tgn_t tryGetNum(const char *fname, const char *arg, int flags,
                                const char *name, long long *out) {
  long long res;
  char *endptr;
  int base;
  int exception = 0;

  if (arg == NULL || *arg == '\0') {
    gnWarn(fname, "null or empty string", arg, name);
    exception = 1;
  } else {

    base = (flags & GN_ANY_BASE)  ? 0
           : (flags & GN_BASE_8)  ? 8
           : (flags & GN_BASE_16) ? 16
                                  : 10;

    errno = 0;
    res = strtoll(arg, &endptr, base);

    /* WARN:(1) Did strtoll() failed? */
    /* Check for overflow/underflow detected by strtoll */
    if (errno != 0) {
      gnWarn(fname, strerror(errno), arg, name);
      exception = 1;
    }

    /* WARN:(2) Did we parse any data? */
    /* Basic sanity: Ensure at least one digit was found */
    else if (endptr == arg) {
      gnWarn(fname, "no digits found", arg, name);
      exception = 1;
    }

    /* WARN:(3) How formatted was the parsed data? */
    /* Ensure the entire string was consumed (no trailing garbage) */
    else if (*endptr != '\0' && !(flags & TGN_IGN_TRAILGB)) {
      if (!(flags & TGN_IGN_TRAILWS) || hasNonWhitespace(endptr)) {
        gnWarn(fname, "nonnumeric characters", arg, name);
        exception = 1;
      }
    }

    /* WARN:(4) Check data quality asked by caller flags. */

    /* Constraint checks based on bitmask flags */
    else if ((flags & GN_NONNEG) && res < 0) {
      gnWarn(fname, "negative value not allowed", arg, name);
      exception = 1;
    } else if ((flags & GN_GT_0) && res <= 0) {
      gnWarn(fname, "value must be > 0", arg, name);
      exception = 1;
    }
  }

  if (exception)
    return TGN_FAILURE;

  *out = res;
  return TGN_SUCCESS;
}

/**
 * Converts string 'arg' to long long.
 * Returns TGN_SUCCESS or TGN_FAILURE.
 */
tgn_t tryGetLLong(const char *arg, int flags, const char *name,
                           long long *out) {
  long long temp;
  tgn_t status = tryGetNum("tryGetLLong", arg, flags, name, &temp);

  if (status == TGN_SUCCESS) {
    if (out == NULL) { /* Gard against dangling pointer (NULL or garbage) */
      /* DEBUG: Message to be removed on production. */
      /* fprintf(stderr, "tryGetLLong: Validation only mode (%s)\n", name); */
    } else
      *out = temp;
  }

  return status;
}

/**
 * Converts string 'arg' to long.
 * Performs range validation against LONG_MAX/LONG_MIN before assignment.
 */
tgn_t tryGetLong(const char *arg, int flags, const char *name,
                          long *out) {
  long long temp;
  tgn_t status = tryGetNum("tryGetLong", arg, flags, name, &temp);

  if (status == TGN_SUCCESS) {
    if (temp > LONG_MAX || temp < LONG_MIN) {
      gnWarn("tryGetLong", "value out of range for 'long'", arg, name);
      return TGN_FAILURE;
    }
    if (out == NULL) { /* Gard against dangling pointer (NULL or garbage) */
      /* DEBUG: Message to be removed on production. */
      /* fprintf(stderr, "tryGetLong: Validation only mode (%s)\n", name); */
    } else
      *out = (long)temp; /* WARN: Cast to gard against stack smashing */
  }
  return status;
}

/**
 * Converts string 'arg' to int.
 * Performs range validation against INT_MAX/INT_MIN before assignment.
 */
tgn_t tryGetInt(const char *arg, int flags, const char *name,
                         int *out) {
  long long temp;
  tgn_t status = tryGetNum("tryGetInt", arg, flags, name, &temp);

  if (status == TGN_SUCCESS) {
    if (temp > INT_MAX || temp < INT_MIN) {
      gnWarn("tryGetInt", "value out of range for 'int'", arg, name);
      return TGN_FAILURE;
    }
    if (out == NULL) { /* Gard against dangling pointer (NULL or garbage) */
      /* DEBUG: Message to be removed on production. */
      /* fprintf(stderr, "tryGetInt: Validation only mode (%s)\n", name); */
    } else
      *out = (int)temp; /* WARN: Cast to gard against stack smashing */
  }
  return status;
}

/* =========================================================================
 * Created on: <Mon Apr 13 20:37:24 +01 2026>
 * Time-stamp: <Fri Jun  5 08:51:03 +01 2026 by owner>
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

/* Private result type: internal transport container to prevent mixing
   signed/unsigned types. */
typedef union {
  long long int pr_s;
  unsigned long long int pr_u;
} privres_t;

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
                       const char *name, int is_unsigned, privres_t *out) {
  privres_t res;
  char *endptr;
  int base;
  int exception = 0;

  if (arg == NULL || *arg == '\0') {
    gnWarn(fname, "null or empty string", arg, name);
    return TGN_FAILURE;
  }
  if (is_unsigned && arg[0] == '-') {
    gnWarn(fname, "negative value not allowed for unsigned type", arg, name);
    return TGN_FAILURE;
  }

  base = (flags & GN_ANY_BASE)  ? 0
         : (flags & GN_BASE_8)  ? 8
         : (flags & GN_BASE_16) ? 16
                                : 10;

  errno = 0;

  if (is_unsigned)
    res.pr_u = strtoull(arg, &endptr, base);
  else
    res.pr_s = strtoll(arg, &endptr, base);

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

  else if (!is_unsigned) {
    /* WARN:(4) Check data quality asked by caller flags. */

    /* Constraint checks based on bitmask flags */
    if ((flags & GN_NONNEG) && res.pr_s < 0) {
      gnWarn(fname, "negative value not allowed", arg, name);
      exception = 1;
    } else if ((flags & GN_GT_0) && res.pr_s <= 0) {
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
 * SIGNED ROUTINES
 */

/**
 * Converts string 'arg' to long long.
 * Returns TGN_SUCCESS or TGN_FAILURE.
 */
tgn_t tryGetLLong(const char *arg, int flags, const char *name,
                  long long *out) {
  privres_t temp;
  tgn_t status = tryGetNum("tryGetLLong", arg, flags, name, 0, &temp);

  if (status == TGN_SUCCESS) {
    if (out == NULL) { /* Gard against dangling pointer (NULL or garbage) */
      /* DEBUG: Message to be removed on production. */
      /* fprintf(stderr, "tryGetLLong: Validation only mode (%s)\n", name); */
    } else
      *out = temp.pr_s;
  }

  return status;
}

/**
 * Converts string 'arg' to long.
 * Performs range validation against LONG_MAX/LONG_MIN before assignment.
 */
tgn_t tryGetLong(const char *arg, int flags, const char *name, long *out) {
  privres_t temp;
  tgn_t status = tryGetNum("tryGetLong", arg, flags, name, 0, &temp);

  if (status == TGN_SUCCESS) {
    if (temp.pr_s > LONG_MAX || temp.pr_s < LONG_MIN) {
      gnWarn("tryGetLong", "value out of range for 'long'", arg, name);
      return TGN_FAILURE;
    }
    if (out == NULL) { /* Gard against dangling pointer (NULL or garbage) */
      /* DEBUG: Message to be removed on production. */
      /* fprintf(stderr, "tryGetLong: Validation only mode (%s)\n", name); */
    } else
      *out = (long)temp.pr_s; /* WARN: Cast to gard against stack smashing */
  }
  return status;
}

/**
 * Converts string 'arg' to int.
 * Performs range validation against INT_MAX/INT_MIN before assignment.
 */
tgn_t tryGetInt(const char *arg, int flags, const char *name, int *out) {
  privres_t temp;
  tgn_t status = tryGetNum("tryGetInt", arg, flags, name, 0, &temp);

  if (status == TGN_SUCCESS) {
    if (temp.pr_s > INT_MAX || temp.pr_s < INT_MIN) {
      gnWarn("tryGetInt", "value out of range for 'int'", arg, name);
      return TGN_FAILURE;
    }
    if (out == NULL) { /* Gard against dangling pointer (NULL or garbage) */
      /* DEBUG: Message to be removed on production. */
      /* fprintf(stderr, "tryGetInt: Validation only mode (%s)\n", name); */
    } else
      *out = (int)temp.pr_s; /* WARN: Cast to gard against stack smashing */
  }
  return status;
}

/**
 * UNSIGNED ROUTINES
 */

/**
 * Converts string 'arg' to unsigned long long.
 * Returns TGN_SUCCESS or TGN_FAILURE.
 */
tgn_t tryGetULLong(const char *arg, int flags, const char *name,
                  unsigned long long *out) {
  privres_t temp;
  tgn_t status = tryGetNum("tryGetLLong", arg, flags, name, 1, &temp);

  if (status == TGN_SUCCESS) {
    if (out == NULL) { /* Gard against dangling pointer (NULL or garbage) */
      /* DEBUG: Message to be removed on production. */
      /* fprintf(stderr, "tryGetLLong: Validation only mode (%s)\n", name); */
    } else
      *out = temp.pr_u;
  }

  return status;
}

/**
 * Converts string 'arg' to unsigned long.
 * Performs range validation against LONG_MAX/LONG_MIN before assignment.
 */
tgn_t tryGetULong(const char *arg, int flags, const char *name, unsigned long *out) {
  privres_t temp;
  tgn_t status = tryGetNum("tryGetLong", arg, flags, name, 1, &temp);

  if (status == TGN_SUCCESS) {
    if (temp.pr_u > ULONG_MAX) {
      gnWarn("tryGetLong", "value out of range for 'long'", arg, name);
      return TGN_FAILURE;
    }
    if (out == NULL) { /* Gard against dangling pointer (NULL or garbage) */
      /* DEBUG: Message to be removed on production. */
      /* fprintf(stderr, "tryGetLong: Validation only mode (%s)\n", name); */
    } else
      *out = (unsigned long)temp.pr_u; /* WARN: Cast to gard against stack smashing */
  }
  return status;
}

/**
 * Converts string 'arg' to int.
 * Performs range validation against INT_MAX/INT_MIN before assignment.
 */
tgn_t tryGetUInt(const char *arg, int flags, const char *name, unsigned int *out) {
  privres_t temp;
  tgn_t status = tryGetNum("tryGetInt", arg, flags, name, 1, &temp);

  if (status == TGN_SUCCESS) {
    if (temp.pr_u > UINT_MAX) {
      gnWarn("tryGetInt", "value out of range for 'int'", arg, name);
      return TGN_FAILURE;
    }
    if (out == NULL) { /* Gard against dangling pointer (NULL or garbage) */
      /* DEBUG: Message to be removed on production. */
      /* fprintf(stderr, "tryGetInt: Validation only mode (%s)\n", name); */
    } else
      *out = (unsigned int)temp.pr_u; /* WARN: Cast to gard against stack smashing */
  }
  return status;
}

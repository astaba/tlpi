/* =========================================================================
 * Created on: <Tue May 05 15:01:52 +01 2026>
 * Time-stamp: <Tue May  5 18:45:27 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details
 * Desc      : ~/coding/c_prog/tlpi/threads/strerror_tsd.c -
 *
 * Listing 31.3: An implementation of strerror() that is made
 * thread-safe through the use of thread-specific data.
 * DEPRECATED:WONTFIX: sys_nerr and sys_errlist are removed from glibc
 * 2.32 on.  This code won't compile.
 * ========================================================================= */
/* Get '_sys_nerr' and '_sys_errlist' declarations from <stdio.h> */
#define _GNU_SOURCE
#include "../lib/tlpi_hdr.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h> /* Get declaration of strerror() */

static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t strerrorKey;

/* Maximum length of string in per-thread buffer returned by strerror() */
#define MAX_ERROR_LEN 256

/* Free thread-specific data buffer: on a thread termination this
   function is paased the non-NULL value once associated with key at
   thread initiation by pthread_getspecific(). */
static void destructor(void *buf) { free(buf); }

static void /* One-time key creation function */
createKey(void) {
  int s;

  /* Allocate a unique thread-specific data key and save the address
     of the destructor for thread-specific data buffers */

  s = pthread_key_create(&strerrorKey, destructor);
  if (s != 0)
    nmsysErr(s, "pthread_key_create");
}

char *strerror(int err) {
  int s;
  char *buf;

  /* Make first caller allocate key for thread-specific data */

  s = pthread_once(&once, createKey);
  if (s != 0)
    nmsysErr(s, "pthread_once");

  buf = pthread_getspecific(strerrorKey);
  if (buf == NULL) { /* If first call from this thread, allocate
                        buffer for thread, and save its location */
    buf = malloc(MAX_ERROR_LEN);
    if (buf == NULL)
      systmErr("malloc");

    /* Now buf is the non-NULL value to associate with key. */
    s = pthread_setspecific(strerrorKey, buf);
    if (s != 0)
      nmsysErr(s, "pthread_setspecific");
  }

  if (err < 0 || err >= _sys_nerr || _sys_errlist[err] == NULL) {
    snprintf(buf, MAX_ERROR_LEN, "Unknown error %d", err);
  } else {
    strncpy(buf, _sys_errlist[err], MAX_ERROR_LEN - 1);
    buf[MAX_ERROR_LEN - 1] = '\0'; /* Ensure null termination */
  }

  return buf;
}

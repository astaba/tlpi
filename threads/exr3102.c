/* =========================================================================
 * Created on: <Tue May 05 20:19:10 +01 2026>
 * Time-stamp: <Wed May  6 18:12:40 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/threads/exr3102.c -
 *
 * Exercise 31.2: Use thread-specific data to write thread-safe
 * versions of dirname() and basename() (Section 18.14).
 *
 * Test this library with [[file:exr3102_test.c]]
 * ========================================================================= */
#include "exr3102_hdr.h"

#define MAX_PATH 4096 /* TSD buf max size */

/* destructor for both TSD versions of ydirname annd ybasename */
static void destructor(void *buf) { free(buf); }

static pthread_once_t once_dname = PTHREAD_ONCE_INIT;
static pthread_once_t once_bname = PTHREAD_ONCE_INIT;
static pthread_key_t dirnameKey;
static pthread_key_t basenamekey;

static void createDirKey(void) { Pthread_key_create(&dirnameKey, destructor); }
static void createBasekey(void) {
  Pthread_key_create(&basenamekey, destructor);
}

/* Thread-safe implementation of dirname() using the Thread-Specific
   Data API. */
char *ydirname(char *path) {
  char *buf, *c;
  Pthread_once(&once_dname, createDirKey);

  buf = pthread_getspecific(dirnameKey);
  if (buf == NULL) {
    buf = calloc(MAX_PATH, sizeof(char));
    if (buf == NULL)
      systmErr("calloc() failed");
    Pthread_setspecific(dirnameKey, buf);
  }

  if (path == NULL || strlen(path) == 0) {
    snprintf(buf, MAX_PATH, ".");
    return buf;
  }

  strncpy(buf, path, MAX_PATH - 1);
  buf[MAX_PATH - 1] = '\0';

  if (strcmp(buf, "/") == 0) {
    snprintf(buf, MAX_PATH, "/");
    return buf;
  }

  /* Ignore trailing slash(es) */
  c = buf + strlen(buf);
  while (strlen(buf) > 1 && *--c == '/')
    *c = '\0';

  c = strrchr(buf, '/');
  if (c == NULL) {
    snprintf(buf, MAX_PATH, ".");
    return buf;
  }

  *c = '\0';
  if (*buf == '\0') {
    snprintf(buf, MAX_PATH, "/");
    return buf;
  }
  return buf;
}

/* Thread-safe implementation of basename() using the Thread-Specific
   Data API. */
char *ybasename(char *path) {
  char *buf, *c;
  Pthread_once(&once_bname, createBasekey);

  buf = pthread_getspecific(basenamekey);
  if (buf == NULL) {
    buf = calloc(MAX_PATH, sizeof(char));
    if (buf == NULL)
      systmErr("calloc() failed");
    Pthread_setspecific(basenamekey, buf);
  }

  if (path == NULL || strlen(path) == 0) {
    snprintf(buf, MAX_PATH, ".");
    return buf;
  }

  strncpy(buf, path, MAX_PATH - 1);
  buf[MAX_PATH - 1] = '\0';

  if (strcmp(buf, "/") == 0) {
    snprintf(buf, MAX_PATH, "/");
    return buf;
  }

  /* Ignore trailing slash(es) */
  c = buf + strlen(buf);
  while (strlen(buf) > 1 && *--c == '/')
    *c = '\0';

  c = strrchr(buf, '/');
  if (c == NULL || strlen(buf) == 1)
    return buf;

  *c = '\0';
  return ++c;
}

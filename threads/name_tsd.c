/* =========================================================================
 * Created on: <Tue May 05 20:19:10 +01 2026>
 * Time-stamp: <Tue May 12 17:14:08 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/threads/name_tsd.c -
 *
 * Exercise 31.2: Use thread-specific data to write thread-safe
 * versions of dirname() and basename() (Section 18.14).
 *
 * Test this library with [[file:exr3102_test.c]]
 * ========================================================================= */
#include "name_tsd.h"
#include <netdb.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATH 4096 /* TSD buf max size */

/* destructor for both TSD versions of ydirname annd ybasename */
static void destructor(void *buf) { free(buf); }

static pthread_once_t once_dname = PTHREAD_ONCE_INIT;
static pthread_key_t dirnameKey;

static void createDirKey(void) {
  int s;
  s = pthread_key_create(&dirnameKey, destructor);
  if (s != 0) {
    fprintf(stderr, "pthread_key_create() failed: %s\n", strerror(s));
    exit(EXIT_FAILURE);
  }
}

/* Thread-safe implementation of dirname() using the Thread-Specific
   Data API. */
char *dirname_tsd(char *path) {
  char *buf, *c;
  int s;
  s = pthread_once(&once_dname, createDirKey);
  if (s != 0) {
    fprintf(stderr, "pthread_once() failed: %s\n", strerror(s));
    exit(EXIT_FAILURE);
  }

  buf = pthread_getspecific(dirnameKey);
  if (buf == NULL) {
    buf = calloc(MAX_PATH, sizeof(char));
    if (buf == NULL) {
      perror("calloc() failed");
      exit(EXIT_FAILURE);
    }
    s = pthread_setspecific(dirnameKey, buf);
    if (s != 0) {
      fprintf(stderr, "pthread_setspecific() failed: %s\n", strerror(s));
      exit(EXIT_FAILURE);
    }
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

static pthread_once_t once_bname = PTHREAD_ONCE_INIT;
static pthread_key_t basenamekey;

static void createBasekey(void) {
  int s;
  s = pthread_key_create(&basenamekey, destructor);
  if (s != 0) {
    fprintf(stderr, "pthread_key_create() failed: %s\n", strerror(s));
    exit(EXIT_FAILURE);
  }
}

/* Thread-safe implementation of basename() using the Thread-Specific
   Data API. */
char *basename_tsd(char *path) {
  char *buf, *c;
  int s;
  s = pthread_once(&once_bname, createBasekey);
  if (s != 0) {
    fprintf(stderr, "pthread_once() failed: %s\n", strerror(s));
    exit(EXIT_FAILURE);
  }

  buf = pthread_getspecific(basenamekey);
  if (buf == NULL) {
    buf = calloc(MAX_PATH, sizeof(char));
    if (buf == NULL) {
      perror("calloc() failed");
      exit(EXIT_FAILURE);
    }
  }
  s = pthread_setspecific(basenamekey, buf);
  if (s != 0) {
    fprintf(stderr, "pthread_setspecific() failed: %s\n", strerror(s));
    exit(EXIT_FAILURE);
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

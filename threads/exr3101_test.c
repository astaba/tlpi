/* =========================================================================
 * Created on: <Wed May 06 15:27:00 +01 2026>
 * Time-stamp: <Wed May  6 18:09:24 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/threads/exr3101_test.c -
 *
 * Test for [[file:exr3101.c]]
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include "exr3101_hdr.h"
#include "pthread_wrappers.h"

/* This section is to test the oncer_t API using the TSD functions
   below */
/* ====================================================================== */
static char *ydirname(char *path);
static char *ybasename(char *path);

static void *threadRoutine(void *arg) {
  char *str = (char *)arg;

  printf("Path: '%s'\tDirname: '%s'\tBasename: '%s'\n", str, ydirname(str),
         ybasename(str));
  return NULL;
}

#define MAX_THREAD 32

int main(int argc, char *argv[argc + 1]) {
  int i;
  pthread_t tids[MAX_THREAD];

  if (argc < 2 || argc > MAX_THREAD + 1) {
    fprintf(stderr, "Usage: %s <path> ... (32 paths at most)\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  for (i = 0; i < argc - 1; i++)
    Pthread_create(&tids[i], NULL, threadRoutine, argv[i + 1]);
  for (i = 0; i < argc - 1; i++)
    Pthread_join(tids[i], NULL);

  return EXIT_SUCCESS;
}

/* Thread-Specific Data Part */
/* ====================================================================== */
#define MAX_PATH 4096

static void destructor(void *buf) { free(buf); }

/* ====================================================================== */
static oncer_t once_dname = ONCE_CONTROL_INITIALIZER;
static pthread_key_t dirnameKey;

static void createDirKey(void) { Pthread_key_create(&dirnameKey, destructor); }

static char *ydirname(char *path) {
  char *buf, *c;
  int s;
  s = one_time_init(&once_dname, createDirKey);
  if (s != 0)
    nmsysErr(s, "pthread_once() failed");

  buf = Pthread_getspecific(dirnameKey);
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

/* ====================================================================== */
static oncer_t once_bname = ONCE_CONTROL_INITIALIZER;
static pthread_key_t basenamekey;

static void createBasekey(void) {
  Pthread_key_create(&basenamekey, destructor);
}

static char *ybasename(char *path) {
  char *buf, *c;
  int s;
  s = one_time_init(&once_bname, createBasekey);
  if (s != 0)
    nmsysErr(s, "pthread_once() failed");

  buf = Pthread_getspecific(basenamekey);
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

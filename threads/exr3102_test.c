/* =========================================================================
 * Created on: <Wed May 06 16:38:50 +01 2026>
 * Time-stamp: <Sat May 23 16:33:49 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/threads/exr3102_test.c -
 *
 * Test for Exercise 31.2 [[file:name_tsd.c][name_tsd]]
 * Use thread-specific data to write thread-safe versions of dirname()
 * and basename() (Section 18.14).
 * ========================================================================= */
#include "../lib/tlpi_hdr.h"
#include "name_tsd.h"
#include "pthread_wrappers.h"

#define MAX_THREAD 32

static void *threadRoutine(void *arg) {
  char *str = (char *)arg;

  printf("Path: '%s'\tDirname: '%s'\tBasename: '%s'\n", str, dirname_tsd(str),
         basename_tsd(str));
  return NULL;
}

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

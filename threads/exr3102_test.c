/* =========================================================================
 * Created on: <Wed May 06 16:38:50 +01 2026>
 * Time-stamp: <Wed May  6 17:24:06 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/threads/exr3102_test.c -
 *
 * Test for [[file:exr3102.c]]
 * ========================================================================= */
#include "exr3102_hdr.h"

#define MAX_THREAD 32

static void *threadRoutine(void *arg) {
  char *str = (char *)arg;

  printf("Path: '%s'\tDirname: '%s'\tBasename: '%s'\n", str, ydirname(str),
         ybasename(str));
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

/* =========================================================================
 * Created on: <Fri Apr 24 03:09:56 +01 2026>
 * Time-stamp: <Fri Apr 24 03:43:39 +01 2026 by owner>
 * Author    : Copyright (C) Michael Kerrisk, 2026.
 *             See the file [[file:../COPYING.gpl-v3]] for details.
 * Desc      : ~/coding/c_prog/tlpi/threads/simple_thread.c -
 *
 * Listing 29.1 A simple POSIX threads example: create a thread, and
 * then join with it.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h"	/* IWYU pragma: keep */
#include <pthread.h>

static void *threadFunc(void *arg) {
  char *s = arg;
  printf("%s", s);
  return (void *)strlen(s);
}

int main(int argc, char *argv[]) {
  pthread_t t1;
  void *res;
  int s;

  s = pthread_create(&t1, NULL, threadFunc, "Hello world\n");
  if (s != 0)
    nmsysErr(s, "pthread_create");

  printf("Message from main()\n");
  s = pthread_join(t1, &res);
  if (s != 0)
    nmsysErr(s, "pthread_join");

  printf("Thread returned %ld\n", (long)res);
  exit(EXIT_SUCCESS);
}

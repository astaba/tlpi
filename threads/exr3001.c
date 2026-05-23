/* =========================================================================
 * Created on: <Mon Apr 27 00:48:05 +01 2026>
 * Time-stamp: <Mon May 18 08:57:36 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/threads/exr3001.c -
 *
 * Exercise 30.1
 *
 * Modify the program in [[file:thread_incr.c]] so that each loop in
 * the thread’s start function outputs the current value of glob and
 * some identifier that uniquely identifies the thread. The unique
 * identifier for the thread could be specified as an argument to the
 * pthread_create() call used to create the thread. For this program,
 * that would require changing the argument of the thread’s start
 * function to be a pointer to a structure containing the unique
 * identifier and a loop limit value. Run the program, redirecting
 * output to a file, and then inspect the file to see what happens to
 * glob as the kernel scheduler alternates execution between the two
 * threads.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include "pthread_wrappers.h"

/* HACK: To increase the race condition probability while keeping the
   loops number low, just increase the number of created threads. With
   500 iterations for each threads, the race of 5 threads is enough to
   fault the glob count (only when scheduler queue really busy.) */
#define PEER_NUM 8

typedef struct {
  int idx;
  int iterNum;
  pthread_t tid;
} arg_t;

static int glob = 0;
/* static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; */

void *threadRoutine(void *arg);

int main(int argc, char *argv[argc + 1]) {
  arg_t args[PEER_NUM];
  int i, iterNum;

  iterNum = (argc > 1) ? getInt(argv[1], GN_GT_0, "iter_num") : 100000L;

  for (i = 0; i < PEER_NUM; i++) {
    args[i].iterNum = iterNum;
    args[i].idx = i;
    Pthread_create(&(args[i].tid), NULL, threadRoutine, args + i);
  }
  for (i = 0; i < PEER_NUM; i++)
    Pthread_join(args[i].tid, NULL);

  printf("\n******************** RESULT ********************\n");
  if (glob == PEER_NUM * iterNum)
    printf("OK:\t%d\n", glob);
  else
    printf("BOOM:\t%d\n", glob);

  exit(EXIT_SUCCESS);
}

void *threadRoutine(void *arg) {
  int iterNum = ((arg_t *)arg)->iterNum;
  int idx = ((arg_t *)arg)->idx;
  int i;

  for (i = 0; i < iterNum; i++) {
    /* Pthread_mutex_lock(&mtx); */
    glob++;
    /* Pthread_mutex_unlock(&mtx); */
    printf("Thread[%d]: glob = %d\n", idx, glob);
  }

  return NULL;
}

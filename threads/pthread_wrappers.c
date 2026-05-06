/* =========================================================================
 * Created on: <Fri Apr 24 21:51:25 +01 2026>
 * Time-stamp: <Wed May  6 18:02:38 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/threads/pthread_wrappers.c -
 * ========================================================================= */
#include "pthread_wrappers.h"

/* WORKING WRAPPERS ===================== */
/* POSIX Thread library functions within these working wrappers
   return 0 on success, or a positive error number on error.
 */

/* Error handling wrapper around pthread_create() */
void Pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                    void *(*start)(void *), void *arg) {
  int s = pthread_create(thread, attr, start, arg);
  if (s != 0)
    nmsysErr(s, "pthread_create() failed");
}

/* Error handling wrapper around pthread_join() */
void Pthread_join(pthread_t thread, void **retval) {
  int s = pthread_join(thread, retval);
  if (s != 0)
    nmsysErr(s, "pthread_join() failed");
}

/* Error handling wrapper around pthread_detach() */
void Pthread_detach(pthread_t thread) {
  int s = pthread_detach(thread);
  if (s != 0)
    nmsysErr(s, "pthread_detach() failed");
}

/* Error handling wrapper around pthread_mutex_lock() */
void Pthread_mutex_lock(pthread_mutex_t *mutex) {
  int s = pthread_mutex_lock(mutex);
  if (s != 0)
    nmsysErr(s, "pthread_mutex_lock() failed");
}

/* Error handling wrapper around pthread_mutex_unlock() */
void Pthread_mutex_unlock(pthread_mutex_t *mutex) {
  int s = pthread_mutex_unlock(mutex);
  if (s != 0)
    nmsysErr(s, "pthread_mutex_unlock() failed");
}

/* Error handling wrapper around pthread_cond_signal() */
void Pthread_cond_signal(pthread_cond_t *cond) {
  int s = pthread_cond_signal(cond);
  if (s != 0)
    nmsysErr(s, "pthread_cond_signal() failed");
}

/* Error handling wrapper around pthread_cond_broadcast() */
void Pthread_cond_broadcast(pthread_cond_t *cond) {
  int s = pthread_cond_broadcast(cond);
  if (s != 0)
    nmsysErr(s, "pthread_cond_broadcast() failed");
}

/* Error handling wrapper around pthread_cond_wait() */
void Pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex) {
  int s = pthread_cond_wait(cond, mutex);
  if (s != 0)
    nmsysErr(s, "pthread_cond_wait() failed");
}

/* Error handling wrapper around pthread_once() */
void Pthread_once(pthread_once_t *once_control, void (*init)(void)) {
  int s = pthread_once(once_control, init);
  if (s != 0)
    nmsysErr(s, "pthread_once() failed");
}

/* Error handling wrapper around pthread_key_create() */
void Pthread_key_create(pthread_key_t *key, void (*destructor)(void *)) {
  int s = pthread_key_create(key, destructor);
  if (s != 0)
    nmsysErr(s, "pthread_key_create() failed");
}

/* Error handling wrapper around pthread_setspecific() */
void Pthread_setspecific(pthread_key_t key, const void *value) {
  int s = pthread_setspecific(key, value);
  if (s != 0)
    nmsysErr(s, "pthread_setspecific() failed");
}

/* CONSISTENCY WRAPPERS ======================= */

/* Consistency wrapper around pthread_exit() */
void Pthread_exit(void *retval) {
  /* Never returns */
  pthread_exit(retval);
}

/* Consistency wrapper around pthread_self() */
pthread_t Pthread_self(void) {
  /* Returns the thread ID of the calling thread */
  return pthread_self();
}

/* Consistency wrapper around pthread_equal() */
int Pthread_equal(pthread_t t1, pthread_t t2) {
  /* Returns nonzero value if t1 and t2 are equal, otherwise 0 */
  return pthread_equal(t1, t2);
}

/* Consistency wrapper around pthread_getspecific(). Returns pointer, or
   NULL if no thread-specific data isassociated with key */
void *Pthread_getspecific(pthread_key_t key) {
  return pthread_getspecific(key);
}

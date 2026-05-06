/* =========================================================================
 * Created on: <Fri Apr 24 22:21:24 +01 2026>
 * Time-stamp: <Wed May  6 18:04:07 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/threads/pthread_wrappers.h -
 * ========================================================================= */
#ifndef PTHREAD_WRAPPERS_H
#define PTHREAD_WRAPPERS_H

#include "../lib/tlpi_hdr.h"
#include <pthread.h>

void Pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                    void *(*start)(void *), void *arg);
void Pthread_join(pthread_t thread, void **retval);
void Pthread_detach(pthread_t thread);
void Pthread_exit(void *retval);
pthread_t Pthread_self(void);
int Pthread_equal(pthread_t t1, pthread_t t2);
void Pthread_mutex_lock(pthread_mutex_t *mutex);
void Pthread_mutex_unlock(pthread_mutex_t *mutex);
void Pthread_cond_signal(pthread_cond_t *cond);
void Pthread_cond_broadcast(pthread_cond_t *cond);
void Pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
void Pthread_once(pthread_once_t *once_control, void (*init)(void));
void Pthread_key_create(pthread_key_t *key, void (*destructor)(void *));
void Pthread_setspecific(pthread_key_t key, const void *value);
void *Pthread_getspecific(pthread_key_t key);

#endif /* PTHREAD_WRAPPERS_H */

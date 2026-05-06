/* =========================================================================
 * Created on: <Wed Apr 29 19:10:09 +01 2026>
 * Time-stamp: <Wed Apr 29 21:35:21 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/psem/psem_wrappers.c -
 * ========================================================================= */
#include "psem_wrappers.h"

/* Error-handling wrapper around sem_open */
sem_t *Sem_Open(const char *name, int oflag, ...) {
  sem_t *sem;
  mode_t mode = 0;
  unsigned int value = 0;

  if (oflag & O_CREAT) {
    va_list ap; /* The proverbial arg_prt */
    va_start(ap, oflag);
    mode = va_arg(ap, mode_t);
    value = va_arg(ap, unsigned int);
    va_end(ap);
  }

  sem = sem_open(name, oflag, mode, value);
  if (sem == SEM_FAILED)
    systmErr("Sem_Open() failed");

  return sem;
}

/* Error-handling wrapper around sem_close */
void Sem_Close(sem_t *sem) {
  if (sem_close(sem) == -1)
    systmErr("Sem_Close() failed");
}

/* Error-handling wrapper around sem_unlink */
void Sem_Unlink(const char *name) {
  if (sem_unlink(name) == -1)
    systmErr("Sem_Unlink() failed");
}

/* Error-handling wrapper around sem_wait */
void Sem_Wait(sem_t *sem) {
  if (sem_wait(sem) == -1)
    systmErr("Sem_Wait() failed");
}

/* Error-handling wrapper around sem_trywait */
void Sem_Trywait(sem_t *sem) {
  if (sem_trywait(sem) == -1)
    systmErr("Sem_Trywait() failed");
}

/* Error-handling wrapper around sem_timedwait */
void Sem_Timedwait(sem_t *sem, const struct timespec *abs_timeout) {
  if (sem_timedwait(sem, abs_timeout) == -1)
    systmErr("Sem_Timedwait() failed");
}

/* Error-handling wrapper around sem_post */
void Sem_Post(sem_t *sem) {
  if (sem_post(sem) == -1)
    systmErr("Sem_Post() failed");
}

/* Error-handling wrapper around sem_getvalue */
void Sem_Getvalue(sem_t *sem, int *sval) {
  if (sem_getvalue(sem, sval) == -1)
    systmErr("Sem_Getvalue() failed");
}

/* Error-handling wrapper around sem_init */
void Sem_Init(sem_t *sem, int pshared, unsigned int value) {
  if (sem_init(sem, pshared, value) == -1)
    systmErr("Sem_Init() failed");
}

/* Error-handling wrapper around sem_destroy */
void Sem_Destroy(sem_t *sem) {
  if (sem_destroy(sem) == -1)
    systmErr("Sem_Destroy() failed");
}

/* =========================================================================
 * Created on: <Wed Apr 29 19:45:13 +01 2026>
 * Time-stamp: <Wed Apr 29 22:03:58 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/psem/psem_wrappers.h -
 * ========================================================================= */
#ifndef PSEM_WRAPPERS_H
#define PSEM_WRAPPERS_H

#define _POSIX_C_SOURCE 200112L /* For sem_timedwait */

#include "../lib/error_functions.h"
#include <fcntl.h> /* Defines O_* constants */
#include <semaphore.h>
#include <stdarg.h>

sem_t *Sem_Open(const char *name, int oflag, ...);
void Sem_Wait(sem_t *sem);
void Sem_Post(sem_t *sem);
void Sem_Getvalue(sem_t *sem, int *sval);
void Sem_Unlink(const char *name);

void Sem_Trywait(sem_t *sem);
void Sem_Timedwait(sem_t *sem, const struct timespec *abs_timeout);
void Sem_Close(sem_t *sem);

void Sem_Init(sem_t *sem, int pshared, unsigned int value);
void Sem_Destroy(sem_t *sem);

#endif /* PSEM_WRAPPERS_H */

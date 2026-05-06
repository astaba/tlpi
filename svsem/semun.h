/* =========================================================================
 * Created on: <Mon Apr 27 19:53:39 +01 2026>
 * Time-stamp: <Mon Apr 27 20:47:01 +01 2026 by owner>
 * Author    : Copyright(C) Michael Kerrisk, 2026 See the files
 *	       [[file:../COPYING.lgpl-v3]] and
 *	       [[file:../COPYING.gpl-v3]] for details
 * Desc      : ~/coding/c_prog/tlpi/svsem/semun.h -
 *
 * Listing 47.2: Definition of the semun union used by the System V
 * semaphore semop() system call.
 * ========================================================================= */
#ifndef SEMUN_H
#define SEMUN_H /* Prevent accidental double inclusion */

#include <sys/sem.h>
#include <sys/types.h> /* For portability */

#if !defined(__FreeBSD__) && !defined(__OpenBSD__) && !defined(__sgi) &&       \
    !defined(__APPLE__)
/* Some implementations already declare this union */

union semun { /* Used in calls to semctl() */
  int val;
  struct semid_ds *buf;
  unsigned short *array;
#if defined(__linux__)
  struct seminfo *__buf;
#endif
};

#endif

#endif	/* SEMUN_H */

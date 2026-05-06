/* =========================================================================
 * Created on: <Mon Apr 27 19:47:27 +01 2026>
 * Time-stamp: <Mon Apr 27 19:49:23 +01 2026 by owner>
 * Author    : Copyright(C) Michael Kerrisk, 2026
 *             See the file [[file:../COPYING.gpl-v3]] for details
 * Desc      : ~/coding/c_prog/tlpi/svsem/svsem_demo.c -
 *
 * Listing 47.1 A simple demonstration of System V semaphores.
 * ========================================================================= */
#include "../lib/curr_time.h" /* Declaration of currTime() */
#include "../lib/tlpi_hdr.h"  /* IWYU pragma: keep */
#include "semun.h"            /* Definition of semun union */
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
  int semid;

  if (argc < 2 || argc > 3 || strcmp(argv[1], "--help") == 0)
    usageErr("%s init-value\n"
             "   or: %s semid operation\n",
             argv[0], argv[0]);

  if (argc == 2) { /* Create and initialize semaphore */
    union semun arg;

    semid = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR);
    if (semid == -1)
      systmErr("semid");

    arg.val = getInt(argv[1], 0, "init-value");
    if (semctl(semid, /* semnum= */ 0, SETVAL, arg) == -1)
      systmErr("semctl");

    printf("Semaphore ID = %d\n", semid);

  } else { /* Perform an operation on first semaphore */

    struct sembuf sop; /* Structure defining operation */

    semid = getInt(argv[1], 0, "semid");

    sop.sem_num = 0; /* Specifies first semaphore in set */
    sop.sem_op = getInt(argv[2], 0, "operation");
    /* Add, subtract, or wait for 0 */
    sop.sem_flg = 0; /* No special options for operation */

    printf("%ld: about to semop at  %s\n", (long)getpid(), currTime("%T"));
    if (semop(semid, &sop, 1) == -1)
      systmErr("semop");

    printf("%ld: semop completed at %s\n", (long)getpid(), currTime("%T"));
  }

  exit(EXIT_SUCCESS);
}

/* Created on: Fri Jan 30 14:29:46 +01 2026 */
/* Description: Practice draft for atomic_append.c */
/* Demonstrate the difference between using of nonatomic lseek(2)+write(2) and
 * the atomic open(2) with O_APPEND. More explanations at end of file about the
 * phenomenon of race condition. */

#include "../lib/tlpi_hdr.h" // IWYU pragma: keep
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

static inline int is_help(const char *h) {
  return (strcmp("--help", h) == 0) || (strcmp("-h", h) == 0);
}
int main(int argc, char *argv[argc + 1]) {
  int fd, flag, numBytes;
  bool useLseek;
  unsigned char c = 0x41;

  if (argc < 3 || is_help(argv[1]))
    usageErr("%s filename numBytes [x]\n"
             "\tx is a flag to choose lseek() over O_APPEND.\n",
             argv[0]);

  useLseek = argc > 3;
  flag = useLseek ? 0 : O_APPEND;
  numBytes = getInt(argv[2], 0, "num-Bytes");

  fd = open(argv[1], O_WRONLY | O_CREAT | flag,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd == -1)
    systmErr("open() failed");

  for (int i = 0; i < numBytes; i++) {
    if (useLseek)
      if (lseek(fd, 0, SEEK_END) == -1)
        systmErr("lseek() failed");
    if (write(fd, &c, 1) != 1)
      systmErr("write() failed");
    c++;
    if (c > 0x5a)
      c = 0x41;
  }

  printf("%ld: done.\n", (long)getpid());
  return EXIT_SUCCESS;
}

/* Test:
  $ ./yatomicappend f1 1000000 & ./yatomicappend f1 1000000
  $ ./yatomicappend f2 1000000 sleep & ./yatomicappend f2 1000000 sleep
*/

/* NOTE:: The RACE CONDITION */
/* First looking at file content it does not seem like much until checking the
 * massive difference in file size. When writing in two function calls using
 * lseek()+write it causes a race-condition: */
/* 1. Process_A calls lseek() and set the offset at 1000 before being preempted
      by scheduler fairness.
   2. Process_B resumes, calls lseek(), sets offset at 1000 and calls write()
   which let the offset at 1001 before scheduler preemption.
   3. Process_A resumes only aware of the offset it set itself at 1000 and
      overwrites Process_B byte at offset 1001.
   4. Process_B takes on from offset at 1001 and so on and so forth. */
/* The Consequence:
For two writes only one byte is actual input. Over a million of iterations the
phenomenon adds up to a massive data deficit. */

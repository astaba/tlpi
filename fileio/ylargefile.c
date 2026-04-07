/* Created on: Fri Jan 30 18:09:01 +01 2026 */
/* Decription: modern version of Listing 5-3 */
/* WARN: This program is Linux-specific. */
/* Purpose: Demonstrate the use of the (modern) Large File Summit (LFS) API.
 * See explanation comments and test run at end of file. */

#define _FILE_OFFSET_BITS 64
#include "../lib/tlpi_hdr.h" // IWYU pragma: keep
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  int fd;
  off_t off;

  if (argc != 3 || strcmp(argv[1], "--help") == 0)
    usageErr("%s pathname offset\n", argv[0]);

  fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
  if (fd == -1)
    systmErr("open");

  /* WARN: Validate input with newly created getLLong() instead of getLong(), or
   * worse atoll(). Validating value above 2^32 with getLong() works on a 64-bit
   * system because LONG_MAX is (2^63 - 1). The moment you do the same on a
   * 32-bit system where LONG_MAX is (2^31 -1) getLong() will trigger gnFail().
   */
  off = getLLong(argv[2], GN_GT_0, argv[0]);

  if (lseek(fd, off, SEEK_SET) == -1)
    systmErr("lseek");

  if (write(fd, "test", 4) == -1)
    systmErr("write");
  exit(EXIT_SUCCESS);
}

/*
The Manual Way (_LARGEFILE64_SOURCE): You have to explicitly use off64_t and
lseek64(). This is tedious and "infects" your whole codebase with "64" suffixes.
The Transparent Way (_FILE_OFFSET_BITS=64): This is the modern standard. It
tells the preprocessor to "alias" the standard functions. When you type lseek(),
the compiler secretly replaces it with the 64-bit version. Your code stays clean
and portable.
2. Why is it "Outdated"
The Large File Summit (LFS) created the ...64 functions as a "transitional" step
while the world was moving from 32-bit to 64-bit. On a 64-bit machine: This is
all irrelevant. off_t is already 64 bits by default. On a 32-bit machine: Using
_FILE_OFFSET_BITS=64 ensures your code works the same way it does on a 64-bit
machine without you changing a single line of C code.

Test:

$ ./draft07 tfile 4294967298    # (2^32 + 2)
$ ls -lh tfile
    -rw------- 1 owner owner 4.1G Jan 30 20:37 tfile
$ du -h tfile
    4.0K    tfile
$ od -c -A x tfile      # After a short while...
    000000  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0  \0
    *
    100000000  \0  \0   t   e   s   t
    100000006
*/

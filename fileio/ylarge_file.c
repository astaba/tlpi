/* =========================================================================
 * Created on: <Fri Jan 30 18:09:01 +01 2026>
 * Time-stamp: <Sun Jun 21 20:57:47 +01 2026 by owner>
 * Author    : W. Richard Stevens and Stephen A. Rago from
 *             "Advanced Programming in the UNIX® Environment" Third Edition
 * Desc      : ~/coding/c_prog/tlpi/fileio/ylarge_file.c -
 * WARN: This program is Linux-specific.
 *
 * Modern Version of Listing 5.3
 * Demonstrate the use of the (modern) Large File Summit (LFS)
 * API. See explanation comments and test run at end of file
 * ========================================================================= */
#define _FILE_OFFSET_BITS 64
#include "tlpi_hdr.h" // IWYU pragma: keep
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

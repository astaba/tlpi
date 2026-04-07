/* Created on: Sat Jan 31 15:05:57 +01 2026 */
/* Description: Section 5.3 Personal Practice */
/* Purpose: Using the fcntl(2) to set open file status flags. */

#include "y_iofunc.h"
#include "../lib/tlpi_hdr.h" // IWYU pragma: keep
#include <fcntl.h>

void ysetfstat(int fd, int flags) {
  int value;
  if ((value = fcntl(fd, F_GETFL)) == -1)
    systmErr("fcntl F_GETFL failed");

  value |= flags;
  if (fcntl(fd, F_SETFL, value) == -1)
    systmErr("fcntl F_SETFL failed");
}

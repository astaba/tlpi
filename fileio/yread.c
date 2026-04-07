/* Created on:  Thu Jan 29 12:50:07 +01 2026 */
/* Description: Experimental snippet from Section 4.4 read(2) */
/* Purpose:     Try the book version first to suffer all the inconveniences of
 *              read(2) binary behavior when used for unsanitized text input.
 *              See SECTION04.md for more explanations.
 */

#include "../lib/tlpi_hdr.h" // IWYU pragma: keep
#include <unistd.h>
#define MAX_READ 5

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  char buffer[MAX_READ + 1];
  ssize_t numRead;

  if ((numRead = read(STDIN_FILENO, buffer, MAX_READ)) == -1)
    systmErr("reading from stdin");

  if (numRead > 0 && buffer[numRead - 1] == '\n') {
    buffer[numRead - 1] = 0;
  } else {
    buffer[numRead] = 0;
    if (numRead) {
      char trash;
      while ((read(STDIN_FILENO, &trash, 1)) > 0 && trash != '\n')
        ;
    }
  }

  printf("The input data was: '%s'\n", buffer);

  return EXIT_SUCCESS;
}

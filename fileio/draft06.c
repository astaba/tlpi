/* Created on: Fri Jan 30 17:19:17 +01 2026 */
/* Description: Practice draft for good exclusive_open */

#include "../lib/tlpi_hdr.h" // IWYU pragma: keep
#include <fcntl.h>
#include <sys/stat.h>

static inline int is_help(const char *h) {
  return (strcmp("--help", h) == 0) || (strcmp("-h", h) == 0);
}

int main(int argc, char *argv[argc + 1]) {
  int fd;
  char buf[128];
  snprintf(buf, 128, "[PID:%ld] Written.\n", (long)getpid());

  if (argc < 2 || is_help(argv[1]))
    usageErr("%s filename\n", argv[0]);

  fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL,
            S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

  if (fd == -1) { /* open() succeeded */
    if (errno == EEXIST)
      printf("[PID:%ld] File \"%s\" already exists. I am not the creator!\n",
             (long)getpid(), argv[1]);
    else
      errExit("open() failed");
  } else { /* open() failed */
    /* Leave temporal opportunity for other process to create file */
    if (argc > 2) {
      sleep(5);
      printf("[PID:%ld] Done sleeping.\n", (long)getpid());
    }

    printf("[PID:%ld] Created \"%s\" file exclusively.\n", (long)getpid(),
           argv[1]);
    if (write(fd, buf, (size_t)strlen(buf)) != (ssize_t)strlen(buf))
      errExit("write() failed");
    close(fd);
  }

  return EXIT_SUCCESS;
}

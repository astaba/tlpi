/* Created on: Fri Jan 30 16:24:57 +01 2026 */
/* Description: Practice draft for bad_exclusive_open.c */

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

  fd = open(argv[1], O_WRONLY);
  if (fd != -1) { /* open() succeeded */
    printf("[PID:%ld] File \"%s\" already exists.\n", (long)getpid(), argv[1]);
    close(fd);
  } else { /* open() failed */
    if (errno != ENOENT)
      errExit("open() failed");

    /* Leave temporal opportunity for other process to create file */
    if (argc > 2) {
      sleep(5);
      printf("[PID:%ld] Done sleeping.\n", (long)getpid());
    }

    fd = open(argv[1], O_WRONLY | O_CREAT | O_APPEND,
              S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1)
      errExit("open() failed");

    printf("[PID:%ld] Created \"%s\" file exclusively.\n", (long)getpid(),
           argv[1]);

    if (write(fd, buf, (size_t)strlen(buf)) != (ssize_t)strlen(buf))
      errExit("write() failed");
    close(fd);
  }

  return EXIT_SUCCESS;
}

/* 1. $ ./draft05 newfile x & ./draft05 newfile &
2. [1] 1295510
3. [2] 1295511
4. [PID:1295511] Created "newfile" file exclusively.
5. [2]+  Done                    ./draft05 newfile
6.
7. $ [PID:1295510] Done sleeping.
8. [PID:1295510] Created "newfile" file exclusively.
9.
10. [1]+  Done                    ./draft05 newfile x */

/* =========================================================================
 * Created on: <Sat Apr 18 21:22:53 +01 2026>
 * Time-stamp: <Sat May  9 20:54:08 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/sockets/yunix_sockets.c -
 *
 * Exercise 59.3
 * A library of useful routines for sockets in the UNIX domain.
 * ========================================================================= */
#include "../lib/tlpi_hdr.h" /* IWYU pragma: keep */
#include "unix_sockets.h"

/* Clear and populate a UNIX address struct.
   Return status code. */
int yunixaddress(const char *path, struct sockaddr_un *addr) {
  if (addr == NULL || path == NULL) {
    errno = EINVAL;
    return -1;
  }

  memset(addr, 0, sizeof(struct sockaddr_un));
  addr->sun_family = AF_UNIX;
  if (strlen(path) < sizeof(addr->sun_path)) {
    strncpy(addr->sun_path, path, sizeof(addr->sun_path));
    return 0;
  } else {
    errno = ENAMETOOLONG;
    return -1;
  }
}

/* Create UNIX socket fd for specified type and connect to address.
   Return socket fd. */
int yunixconnect(const char *path, struct sockaddr_un *addr, int type) {
  int sfd, savedErrno;
  socklen_t socklen;

  if (yunixaddress(path, addr) == -1)
    return -1;

  if ((sfd = socket(PF_UNIX, type, 0)) == -1)
    return -1;

  socklen = SUN_LEN(addr);
  if (connect(sfd, (struct sockaddr *)addr, socklen) == -1) {
    savedErrno = errno;
    close(sfd); /* Might change 'errno' */
    errno = savedErrno;
    return -1;
  }

  return sfd;
}

/* Create UNIX socket fd for specified type and bind to address after
   making sure the path is created anew.  Return socket fd. */
int yunixbind(const char *path, struct sockaddr_un *addr, int type) {
  int sfd, savedErrno;
  socklen_t socklen;

  if (strlen(path) + 1 > sizeof(addr->sun_path)) {
    errno = ENAMETOOLONG;
    return -1;
  }
  if (remove(path) == -1 && errno != ENOENT)
    return -1;

  if (yunixaddress(path, addr) == -1)
    return -1;

  if ((sfd = socket(PF_UNIX, type, 0)) == -1)
    return -1;

  socklen = SUN_LEN(addr);
  if (bind(sfd, (struct sockaddr *)addr, socklen) == -1) {
    savedErrno = errno;
    close(sfd); /* Might change 'errno' */
    errno = savedErrno;
    return -1;
  }

  return sfd;
}

/* ===========================================================================
 API for the abstract namespace. Exclusively for Linux systems
============================================================================= */

/* Clear and populate a UNIX address struct in the abstract namespace
   Return status code. */
int yunixAaddress(const char *path, struct sockaddr_un *addr) {
  if (addr == NULL || path == NULL) {
    errno = EINVAL;
    return -1;
  }

  memset(addr, 0, sizeof(struct sockaddr_un));
  addr->sun_family = AF_UNIX;
  if ( 1 + strlen(path) < sizeof(addr->sun_path)) {
    strncpy(addr->sun_path + 1, path, sizeof(addr->sun_path));
    return 0;
  } else {
    errno = ENAMETOOLONG;
    return -1;
  }
}

/* Create UNIX socket fd for specified type and connect to address in
   the abstract namespace.  Return socket fd. */
int yunixAconnect(const char *path, struct sockaddr_un *addr, int type) {
  int sfd, savedErrno;
  socklen_t socklen;

  if (yunixAaddress(path, addr) == -1)
    return -1;

  if ((sfd = socket(PF_UNIX, type, 0)) == -1)
    return -1;

  socklen = offsetof(struct sockaddr_un, sun_path) + 1 + strlen(path);
  if (connect(sfd, (struct sockaddr *)addr, socklen) == -1) {
    savedErrno = errno;
    close(sfd); /* Might change 'errno' */
    errno = savedErrno;
    return -1;
  }

  return sfd;
}

/* Create UNIX socket fd for specified type and bind to address in the
   abstract namespace.  Return socket fd. */
int yunixAbind(const char *path, struct sockaddr_un *addr, int type) {
  int sfd, savedErrno;
  socklen_t socklen;

  if (yunixAaddress(path, addr) == -1)
    return -1;

  if ((sfd = socket(PF_UNIX, type, 0)) == -1)
    return -1;

  socklen = offsetof(struct sockaddr_un, sun_path) + 1 + strlen(path);
  if (bind(sfd, (struct sockaddr *)addr, socklen) == -1) {
    savedErrno = errno;
    close(sfd); /* Might change 'errno' */
    errno = savedErrno;
    return -1;
  }

  return sfd;
}

/* =========================================================================
 * Created on: <Wed May 06 13:40:54 +01 2026>
 * Time-stamp: <Wed May  6 17:34:31 +01 2026 by owner>
 * Author    : owner
 * Desc      : ~/coding/c_prog/tlpi/threads/exr3101.c -
 *
 * Exercise 31.1: Implement a function, one_time_init(control, init),
 * that performs the equivalent of pthread_once(). The control
 * argument should be a pointer to a statically allocated structure
 * containing a Boolean variable and a mutex. The Boolean variable
 * indicates whether the function init has already been called, and
 * the mutex controls access to that variable. To keep the
 * implementation simple, you can ignore possibilities such as init()
 * failing or being canceled when first called from a thread (i.e., it
 * is not necessary to devise a scheme whereby, if such an event
 * occurs, the next thread that calls one_time_init() reattempts the
 * call to init(). HACK: the bool is set true AFTER running init()
 * precisely to leave the door open for the option).
 *
 * Test with [[file:exr3101_test.c]]
 * ========================================================================= */
#include "exr3101_hdr.h"

int one_time_init(oncer_t *control, void (*init)(void)) {
  int s;
  if (control->once)
    return 0;

  s = pthread_mutex_lock(&control->mutex);
  if (s != 0)
    return s;

  if (control->once == false) {
    init();
    control->once = true; /* set after init() for safety */
  }

  s = pthread_mutex_unlock(&control->mutex);
  if (s != 0)
    return s;

  return s;
}

include ../Makefile.inc

GEN_EXE = detached_attrib one_time_init prod_condvar prod_no_condvar \
	pthread_barrier_demo \
	simple_thread \
	thread_cancel thread_cleanup thread_incr thread_incr_mutex \
	thread_incr_rwlock thread_incr_spinlock \
	thread_lock_speed \
	thread_multijoin \
	exr2901 \
	ythread_incr ythread_incr_mutex \
	exr3001 exr3102 exr3101_test exr3102_test

LINUX_EXE =

EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

CFLAGS = ${IMPL_CFLAGS} ${IMPL_THREAD_FLAGS}
LDLIBS = ${IMPL_LDLIBS} ${IMPL_THREAD_FLAGS}

strerror_test: strerror_test.o strerror.o
	${CC} -o $@ strerror_test.o strerror.o \
		${CFLAGS} ${LDLIBS}

strerror_test_tsd: strerror_test.o strerror_tsd.o
	${CC} -o $@ strerror_test.o strerror_tsd.o \
		${CFLAGS} ${LDLIBS}

strerror_test_tls: strerror_test.o strerror_tls.o
	${CC} -o $@ strerror_test.o strerror_tls.o \
		${CFLAGS} ${LDLIBS}

exr3101_test: exr3101_test.o exr3101.o
	$(CC) -o $@ $^ ${CFLAGS} ${LDLIBS}
exr3101_test.o exr3101.o: exr3101_hdr.h

exr3102_test: exr3102_test.o exr3102.o
	$(CC) -o $@ $^ ${CFLAGS} ${LDLIBS}
exr3102_test.o exr3102.o: exr3102_hdr.h

clean :
	${RM} ${EXE} *.o *~

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation

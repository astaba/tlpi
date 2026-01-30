include ../Makefile.inc

CFLAGS = -std=c2x \
				-D_DEFAULT_SOURCE \
				-g -I${TLPI_INCL_DIR} \
				-Wall -Wextra -pedantic

GEN_EXE = atomic_append bad_exclusive_open copy \
	multi_descriptors seek_io t_readv t_truncate \
	draft01 draft02 draft03 draft04 draft05 \
	draft06 draft07

LINUX_EXE = large_file

EXE = ${GEN_EXE} ${LINUX_EXE}

TEMP = tfile f1 f2 newfile

all : ${EXE}

allgen : ${GEN_EXE}


clean :
	${RM} ${TEMP} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation

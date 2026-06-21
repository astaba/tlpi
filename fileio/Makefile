include ../Makefile.inc

CFLAGS = -std=c2x \
				-D_DEFAULT_SOURCE \
				-g -I${TLPI_INCL_DIR} \
				-Wall -Wextra -pedantic

GEN_EXE = \
	copy \
	ycopy \
	seek_io \
	yseek_io \
	exr0401 \
	exr0402 \
	atomic_append \
	bad_exclusive_open \
	multi_descriptors \
	t_readv \
	t_truncate

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

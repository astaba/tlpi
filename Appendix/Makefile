include ../Makefile.inc

CFLAGS = -std=c2x \
	-D_DEFAULT_SOURCE \
	-g -I${TLPI_INCL_DIR} \
	-Wall -Wextra -pedantic

GEN_EXE = \
	t_getopt

LINUX_EXE =

EXE = ${GEN_EXE} ${LINUX_EXE}

TEMP =

all : ${EXE}

allgen : ${GEN_EXE}


clean :
	${RM} ${TEMP} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation

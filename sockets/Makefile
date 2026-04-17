include ../Makefile.inc

GEN_EXE = i6d_ucase_sv i6d_ucase_cl \
	id_echo_cl id_echo_sv \
	is_echo_cl is_echo_sv is_echo_inetd_sv is_echo_v2_sv \
	is_seqnum_sv is_seqnum_cl is_seqnum_v2_sv is_seqnum_v2_cl \
	yis_seqnum_sv yis_seqnum_cl yis_seqnum_v2_sv yis_seqnum_v2_cl \
	socknames t_gethostbyname t_getservbyname \
	ud_ucase_sv ud_ucase_cl \
	yud_ucase_sv yud_ucase_cl \
	us_xfr_cl us_xfr_sv us_xfr_v2_cl us_xfr_v2_sv \
	yus_xfr_cl yus_xfr_sv yus_xfr_v2_cl yus_xfr_v2_sv \
	exr5701_cl exr5701_sv \
	yi6d_ucase_sv yi6d_ucase_cl

LINUX_EXE = list_host_addresses \
	scm_cred_recv scm_cred_send \
	scm_multi_recv scm_multi_send \
	scm_rights_recv scm_rights_send \
	us_abstract_bind \
	exr5702_cl exr5702_sv \
	exr5704_a exr5704_b exr5704_c

EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}

i6d_ucase_sv.o i6d_ucase_cl.o : i6d_ucase.h
yi6d_ucase_sv.o yi6d_ucase_cl.o : i6d_ucase.h

id_echo_cl.o id_echo_sv.o : id_echo.h

is_seqnum_sv.o is_seqnum_cl.o : is_seqnum.h
yis_seqnum_sv.o yis_seqnum_cl.o : is_seqnum.h

is_seqnum_v2_sv.o is_seqnum_v2_cl.o : is_seqnum_v2.h
yis_seqnum_v2_sv.o yis_seqnum_v2_cl.o : is_seqnum_v2.h

scm_cred_recv.o scm_cred_send.o : scm_cred.h

scm_multi_recv.o scm_multi_send.o : scm_multi.h

scm_rights_recv.o scm_rights_send.o : scm_rights.h

us_xfr_sv.o us_xfr_cl.o : us_xfr.h
exr5702_sv.o exr5702_cl.o : us_xfr.h

us_xfr_v2_sv.o us_xfr_v2_cl.o : us_xfr_v2.h

ud_ucase_sv.o ud_ucase_cl.o : ud_ucase.h
yud_ucase_sv.o yud_ucase_cl.o : ud_ucase.h
exr5701_sv.o exr5701_cl.o : ud_ucase.h

exr5704_a.o exr5704_b.o exr5704_c.o: exr5704.h

clean :
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation

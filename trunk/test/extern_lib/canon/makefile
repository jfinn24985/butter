##START:version
#Butter version 1.2beta.175
##END:version
##START:date
#Tue Dec 12 00:01:51 2017
##END:date
##START:preamble
ROOTDIR := /home/finnerty/src/butter/trunk/test/extern_lib/output
OUTPUTDIR := /home/finnerty/src/butter/trunk/test/extern_lib/output
include $(ROOTDIR)/M_sys.mk

##END:preamble
##START:target program
__PROGRAMSRC :=  \
	example_exe.cc
__PROGRAM_DINC := $(filter %$(sufdep), $(__PROGRAMSRC:.cc=$(sufdep))) $(filter %$(sufdep), $(__PROGRAMSRC:.c=$(sufdep)))
program$(sufexe): LDFLAGS+= -L${QTDIR}/lib64 -lqt-mt
program$(sufexe) $(__PROGRAM_DINC) : CPPFLAGS+=-I${QTDIR}/include -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT 
__PROGRAM_OBJ+=$(filter %$(sufobj), $(__PROGRAMSRC:.cc=$(sufobj)))
USEROBJ+=$(__PROGRAM_OBJ)
program$(sufexe) : $(__PROGRAM_SRC) $(__PROGRAM_DINC)
-include $(__PROGRAM_DINC)
$(eval $(call do_link_exe,program$(sufexe),$(__PROGRAM_OBJ)))

all :: program$(sufexe)
TARGETS+=program$(sufexe)

$(eval $(call do_install,program$(sufexe),BIN))

##END:target program

##START:version
#Butter version 1.2beta.175
##END:version
##START:date
#Tue Mar 20 21:38:04 2018
##END:date
##START:preamble
ROOTDIR := /home/finnerty/src/butter/trunk/test/extern_lib2/output
OUTPUTDIR := /home/finnerty/src/butter/trunk/test/extern_lib2/output
include $(ROOTDIR)/M_sys.mk

##END:preamble
include $(ROOTDIR)/qt.mk
##START:target program
__PROGRAMSRC :=  \
	example_exe.cc
__PROGRAM_DINC := $(filter %$(sufdep), $(__PROGRAMSRC:.cc=$(sufdep))) $(filter %$(sufdep), $(__PROGRAMSRC:.c=$(sufdep)))
program$(sufexe): LDFLAGS+= $(QTLIB)
program$(sufexe): CCCFLAGS+=$(QTFLAGS) 
program$(sufexe): CCFLAGS+=$(QTFLAGS) 
__PROGRAM_OBJ+=$(filter %$(sufobj), $(__PROGRAMSRC:.cc=$(sufobj)))
USEROBJ+=$(__PROGRAM_OBJ)
program$(sufexe) : $(__PROGRAM_SRC) $(__PROGRAM_DINC)
-include $(__PROGRAM_DINC)
$(eval $(call do_link_exe,program$(sufexe),$(__PROGRAM_OBJ)))

all :: program$(sufexe)
TARGETS+=program$(sufexe)

$(eval $(call do_install,program$(sufexe),BIN))

##END:target program

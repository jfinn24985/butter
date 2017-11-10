##START:version
#Butter version 1.2beta.175
##END:version
##START:date
#Wed Nov 8 20:00:39 2017
##END:date
##START:preamble
include $(ROOTDIR)/M_sys.mk

##END:preamble
##START:target butter
BUTTER_BUTTERSRC :=  \
	jam_generator.cpp \
	butter_constants.cpp \
	utility.cpp \
	bjam_generator.cpp \
	compound_artifact.cpp \
	location.cpp \
	base_generator.cpp \
	basic_style.cpp \
	gmake_generator.cpp \
	cmake_generator.cpp \
	compound_document.cpp \
	style.cpp
BUTTER_BUTTER_DINC := $(filter %$(sufdep), $(BUTTER_BUTTERSRC:.cpp=$(sufdep))) $(filter %$(sufdep), $(BUTTER_BUTTERSRC:.c=$(sufdep)))
butter$(suflib): LDFLAGS+= -L${QTDIR}/lib64 -lqt-mt -lm
butter$(suflib) $(BUTTER_BUTTER_DINC) : CPPFLAGS+=-I${QTDIR}/include -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT 
BUTTER_BUTTER_OBJ+=$(filter %$(sufobj), $(BUTTER_BUTTERSRC:.cpp=$(sufobj)))
USEROBJ+=$(BUTTER_BUTTER_OBJ)
butter$(suflib) : $(BUTTER_BUTTER_SRC) $(BUTTER_BUTTER_DINC)
-include $(BUTTER_BUTTER_DINC)
$(eval $(call do_archive,butter$(suflib),$(BUTTER_BUTTER_OBJ)))

all :: butter$(suflib)
TARGETS+=butter$(suflib)

$(eval $(call do_install,butter$(suflib),LIB))

##END:target butter
##START:target test_path
BUTTER_TEST_PATHSRC :=  \
	path_test.cpp \
	utility.cpp
BUTTER_TEST_PATH_DINC := $(filter %$(sufdep), $(BUTTER_TEST_PATHSRC:.cpp=$(sufdep))) $(filter %$(sufdep), $(BUTTER_TEST_PATHSRC:.c=$(sufdep)))
test_path$(sufexe): LDFLAGS+= -L${QTDIR}/lib64 -lqt-mt -lm
test_path$(sufexe) $(BUTTER_TEST_PATH_DINC) : CPPFLAGS+=-I${QTDIR}/include -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT 
BUTTER_TEST_PATH_OBJ+=$(ROOTDIR)/src/bouml/bouml$(suflib)

path_test$(sufobj) : CCCFLAGS+=-fopenmp 


BUTTER_TEST_PATH_OBJ+=$(filter %$(sufobj), $(BUTTER_TEST_PATHSRC:.cpp=$(sufobj)))
USEROBJ+=$(BUTTER_TEST_PATH_OBJ)
test_path$(sufexe) : $(BUTTER_TEST_PATH_SRC) $(BUTTER_TEST_PATH_DINC)
-include $(BUTTER_TEST_PATH_DINC)
$(eval $(call do_link_exe,test_path$(sufexe),$(BUTTER_TEST_PATH_OBJ)))

all :: test_path$(sufexe)
TARGETS+=test_path$(sufexe)

$(eval $(call do_install,test_path$(sufexe),BIN))

##END:target test_path
##START:target butter_exe
BUTTER_BUTTER_EXESRC :=  \
	main.cpp
BUTTER_BUTTER_EXE_DINC := $(filter %$(sufdep), $(BUTTER_BUTTER_EXESRC:.cpp=$(sufdep))) $(filter %$(sufdep), $(BUTTER_BUTTER_EXESRC:.c=$(sufdep)))
butter_exe$(sufexe): LDFLAGS+= -L${QTDIR}/lib64 -lqt-mt -lm
butter_exe$(sufexe) $(BUTTER_BUTTER_EXE_DINC) : CPPFLAGS+=-I${QTDIR}/include -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT 
BUTTER_BUTTER_EXE_OBJ+=$(ROOTDIR)/src/bouml/bouml$(suflib)
BUTTER_BUTTER_EXE_OBJ+=$(ROOTDIR)/src/butter/butter$(suflib)

BUTTER_BUTTER_EXE_OBJ+=$(filter %$(sufobj), $(BUTTER_BUTTER_EXESRC:.cpp=$(sufobj)))
USEROBJ+=$(BUTTER_BUTTER_EXE_OBJ)
butter_exe$(sufexe) : $(BUTTER_BUTTER_EXE_SRC) $(BUTTER_BUTTER_EXE_DINC)
-include $(BUTTER_BUTTER_EXE_DINC)
$(eval $(call do_link_exe,butter_exe$(sufexe),$(BUTTER_BUTTER_EXE_OBJ)))

all :: butter_exe$(sufexe)
TARGETS+=butter_exe$(sufexe)

$(eval $(call do_install,butter_exe$(sufexe),BIN))

##END:target butter_exe

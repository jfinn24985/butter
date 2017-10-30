##START:version
#Butter version 1.2beta.175
##END:version
##START:date
#Sat Oct 28 23:45:14 2017
##END:date
##START:preamble
include $(ROOTDIR)/M_sys.mk

##END:preamble
##START:target butter_exec
BUTTER_TEST_BUTTER_EXECSRC :=  \
	core_test_suite.cpp
BUTTER_TEST_BUTTER_EXEC_DINC := $(filter %$(sufdep), $(BUTTER_TEST_BUTTER_EXECSRC:.cpp=$(sufdep))) $(filter %$(sufdep), $(BUTTER_TEST_BUTTER_EXECSRC:.c=$(sufdep)))
butter_exec$(sufexe): LDFLAGS+= -L${QTDIR}/lib64 -lqt-mt -lm $(BOOST_LIB) -lboost_unit_test_framework
butter_exec$(sufexe) $(BUTTER_TEST_BUTTER_EXEC_DINC) : CPPFLAGS+=-I${QTDIR}/include -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT -DBOOST_TEST_DYN_LINK 
BUTTER_TEST_BUTTER_EXEC_OBJ+=$(ROOTDIR)/src/bouml/bouml$(suflib)
BUTTER_TEST_BUTTER_EXEC_OBJ+=$(ROOTDIR)/src/butter/butter$(suflib)

BUTTER_TEST_BUTTER_EXEC_OBJ+=$(filter %$(sufobj), $(BUTTER_TEST_BUTTER_EXECSRC:.cpp=$(sufobj)))
USEROBJ+=$(BUTTER_TEST_BUTTER_EXEC_OBJ)
butter_exec$(sufexe) : $(BUTTER_TEST_BUTTER_EXEC_SRC) $(BUTTER_TEST_BUTTER_EXEC_DINC)
-include $(BUTTER_TEST_BUTTER_EXEC_DINC)
$(eval $(call do_link_exe,butter_exec$(sufexe),$(BUTTER_TEST_BUTTER_EXEC_OBJ)))

all :: butter_exec$(sufexe)
TARGETS+=butter_exec$(sufexe)

##END:target butter_exec
##START:target plugout_exec
BUTTER_TEST_PLUGOUT_EXECSRC :=  \
	plugout_test_suite.cpp
BUTTER_TEST_PLUGOUT_EXEC_DINC := $(filter %$(sufdep), $(BUTTER_TEST_PLUGOUT_EXECSRC:.cpp=$(sufdep))) $(filter %$(sufdep), $(BUTTER_TEST_PLUGOUT_EXECSRC:.c=$(sufdep)))
plugout_exec$(sufexe): LDFLAGS+= -L${QTDIR}/lib64 -lqt-mt -lm $(BOOST_LIB) -lboost_unit_test_framework
plugout_exec$(sufexe) $(BUTTER_TEST_PLUGOUT_EXEC_DINC) : CPPFLAGS+=-I${QTDIR}/include -DQT_NO_DEBUG -DQT_SHARED -DQT_THREAD_SUPPORT -DBOOST_TEST_DYN_LINK 
BUTTER_TEST_PLUGOUT_EXEC_OBJ+=$(ROOTDIR)/src/bouml/bouml$(suflib)
BUTTER_TEST_PLUGOUT_EXEC_OBJ+=$(ROOTDIR)/src/butter/butter$(suflib)

BUTTER_TEST_PLUGOUT_EXEC_OBJ+=$(filter %$(sufobj), $(BUTTER_TEST_PLUGOUT_EXECSRC:.cpp=$(sufobj)))
USEROBJ+=$(BUTTER_TEST_PLUGOUT_EXEC_OBJ)
plugout_exec$(sufexe) : $(BUTTER_TEST_PLUGOUT_EXEC_SRC) $(BUTTER_TEST_PLUGOUT_EXEC_DINC)
-include $(BUTTER_TEST_PLUGOUT_EXEC_DINC)
$(eval $(call do_link_exe,plugout_exec$(sufexe),$(BUTTER_TEST_PLUGOUT_EXEC_OBJ)))

all :: plugout_exec$(sufexe)
TARGETS+=plugout_exec$(sufexe)

##END:target plugout_exec

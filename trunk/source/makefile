##START:version
#Butter version 1.2beta.175
##END:version
##START:date
#Tue Oct 31 13:07:07 2017
##END:date
##START:preamble
ROOTDIR := /home/finnerty/src/butter/trunk/source
OUTPUTDIR := /home/finnerty/src/butter/trunk/source
include $(ROOTDIR)/M_sys.mk

CPPFLAGS+=-I$(ROOTDIR)/include/bouml -I$(ROOTDIR)/include -DWITHCPP -DDEBUG 
CCFLAGS+=-DWITHCPP -DDEBUG
CCCFLAGS+=-DWITHCPP -DDEBUG
##END:preamble
##START:target README.html
-include $(ROOTDIR)/t2t.mak

-include $(ROOTDIR)/t2t.make

___README_HTMLSRC :=  \
	README.t2t
README.html : $(___README_HTMLSRC) 
	$(T2T) $(T2TFLAGS) README.html $(___README_HTMLSRC)


all :: README.html
TARGETS+=README.html

$(eval $(call do_install,README.html,DOC,FILE))

##END:target README.html
##START:target bouml
dirs += src/bouml
##END:target bouml
##START:target butter
dirs += src/butter
##END:target butter
##START:target butter_test
dirs += src/butter/butter_test
##END:target butter_test

####################################
##  Definitions for GNU g++ compiler
####################################
ifndef M_GCC_MK
M_GCC_MK:=1
##
## Define compiler variables once only.
## 
CCC?=g++
FORTRAN?=gfortran
CPPFLAGS?=
CCFLAGS+=-Wall -std=c99
CCCFLAGS+=-Wall -std=c++11
CPPFLAGS+=-Wall -std=c++11

ifeq ($(VARIANT),DEBUG)
OPTFLAGS+=-O0 -DDEBUG=1 -gdwarf-2
CCFLAGS+=-pedantic
# -Weffc++
else
OPTFLAGS+=-O2 -march=native -DDEBUG=0
endif

CCFLAGS+=$(OPTFLAGS)
CCCFLAGS+=$(OPTFLAGS)
FORTRANFLAGS+=$(OPTFLAGS)
LDFLAGS?=
SHRFLAGS?=-fpic

FORTRANLIBS:=-lgfortran
OPENMP:=-fopenmp

endif
# END ONCE-ONLY

define _makedep_
%$(sufdep): %$(1)
	set -e; $$(CCC) -MM $$(CPPFLAGS) $$< \
	| sed 's/\($$*\)$$(sufobj)[ :]*/\1$$(sufobj) $$@ : /g' > $$@;\
	[ -s $$@ ] || rm -f $$@
endef

$(foreach suff,.c .C .cc .cpp .cxx,$(eval $(call _makedep_,$(suff))))
_makedep_:=

define do_link_exe
$(1) : $(2); $$(LINK.cpp) $$(OUTPUT_OPTIONS) -o $(1) -Wl,--start-group $(2) -Wl,--end-group
endef
define do_link_shr
$(1) : CCFLAGS+=$(SHRFLAGS)
$(1) : CCCFLAGS+=$(SHRFLAGS)
$(1) : $(2); $$(LINK.cpp) $$(OUTPUT_OPTIONS) -shared -Wl,-soname,$(1) -o $(1) -Wl,--start-group $(2) -Wl,--end-group
endef


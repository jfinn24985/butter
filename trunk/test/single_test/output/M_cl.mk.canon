##########################################
##  Definitions for Microsoft VC compiler
#########################################
ifndef M_CL_MK
M_CL_MAK:=1

CCC:=cl
CXX:=cl
AR:=link
ARFLAGS:= /LIB
FORTRAN:=fc
CPPFLAGS?=
CCFLAGS+=/Za /nologo /errorReport:none
CCCFLAGS+=/GR /Gm- /EHsc /Za /Zc:forScope /nologo /errorReport:none

ifeq ($(VARIANT),DEBUG)
OPTFLAGS+=/Od /Zi
LDFLAGS+=/MTd
else
OPTFLAGS+=/O2
LDFLAGS+=/MT
endif

CCFLAGS+=$(OPTFLAGS)
CCCFLAGS+=$(OPTFLAGS)
FORTRANFLAGS?=
LDFLAGS+=$(OPTFLAGS)

FORTRANLIBS:=
OPENMP:=/openmp

endif # end of once-only

#  commands to execute (built-in):
COMPILE.c = $(CC) $(subst -,/,$(subst -L,/LIBPATH ,$(patsubst -l%,%.lib,$(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH)))) /c /Fo
COMPILE.cc = $(CXX) $(subst -,/,$(subst -L,/LIBPATH ,$(patsubst -l%,%.lib,$(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH)))) /c /Fo
define _makeobj_
%$$(sufobj): %$(1) ; $$(COMPILE$(1))$$@ $$<
endef

$(foreach suff,.c .cc .cpp .cxx,$(eval $(call _makeobj_,$(suff))))

# Commands to create dummy make dependency files.
# (Or use depend generator such as X11 makedepend
# or Digital Mars makedep or makedep.py)
# %$(sufdep): %$(1)
# 	start makedep.py $$@ $$< $(CC) /showInclude /Zs /nologo $(CPPFLAGS) $(CXXFLAGS)
define _makedep_
%$(sufdep): %$(1) ; @echo # Dummy file  $$@
endef

$(foreach suff,.c .C .cc .cpp .cxx,$(eval $(call _makedep_,$(suff))))

_makeobj_:=
_makedep_:=

define do_link_exe
$(1) : $(2); $$(LINK.cpp) $$(OUTPUT_OPTIONS) /Fe$(1) $(2)
endef

define do_link_shr
$(1) : CCFLAGS+=$(SHRFLAGS)
$(1) : CCCFLAGS+=$(SHRFLAGS)
$(1) : $(2); $$(LINK.cpp) $$(OUTPUT_OPTIONS) /dll /Fe$(1) $(2)
endef

define do_archive
$(1): $(2) ; $$(AR) $$(ARFLAGS) /OUT:$(1) $(2)
endef


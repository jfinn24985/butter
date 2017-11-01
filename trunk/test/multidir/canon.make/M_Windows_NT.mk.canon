# Definition of for loop in local shell.

define doit
IF "$(2)" NEQ "" FOR %%W IN ( $(2) ) DO "$(subst /,\,$(MAKE))" -C %%W $(1)
endef

ifndef M_WINNT_MK
M_WINNT_MK:=1
#############################
##  Default install locations
#############################
PREFIX?=$(ROOTDIR)\installdir
BINDIR?=$(PREFIX)\bin
DATADIR?=$(PREFIX)\share
DOCDIR?=$(DATADIR)\doc
HTMLDIR?=$(DATADIR)\html
INCLUDEDIR?=$(PREFIX)\include
LIBDIR?=$(PREFIX)\lib
MANDIR?=$(DATADIR)\man1

# Standard Suffixes
sufobj:=.obj
sufexe:=.exe
sufdep:=.dep
suflib:=.lib
sufshr:=.dll
# Programs
MKDIR?=MD
COPY?=COPY /Y /B /V
RM:=-DEL /F

# Path separator
SLASH?=\

# END OF ONCE-ONLY.
endif

define do_install
install:: $(1) ; IF NOT EXIST $$($(2)DIR) ( $$(MKDIR) $$($(2)DIR) )
	$$(COPY) $(1) $$($(2)DIR)\$(1)
endef


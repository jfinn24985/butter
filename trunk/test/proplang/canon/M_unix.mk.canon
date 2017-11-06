 # Definition of for loop in local shell.
define doit
$(if $(2),STATUS=0 ; for subdir in $(2); do $(MAKE) -C $${subdir}  $(1) || STATUS=$$? ; done ; exit $${STATUS})
endef

# MAKE MOST VARIABLES ONCE-ONLY
ifndef M_UNIX_MK
M_UNIX_MK:=1
#############################
##  Default install locations
#############################
PREFIX=$(ROOTDIR)/installdir
BINDIR?=$(PREFIX)/bin
LIBDIR?=$(PREFIX)/bin
INCLUDEDIR?=$(PREFIX)/include
DATADIR?=$(PREFIX)/share
DOCDIR?=$(DATADIR)/doc
MANDIR?=$(DATADIR)/man1
HTMLDIR?=$(DATADIR)/html

# Flags for the install targets.
BINIFLAGS?=-m 755
FILEIFLAGS?=-m 644
LIBIFLAGS?=-m 644 
MANIFLAGS?=-m 644
INSTALL?=install

# Standard suffixes
sufobj:=.o
sufexe:=
suflib:=.a
sufshr:=.so
sufdep:=.d

# Path separator
SLASH?=/

# END OF ONCE-ONLY.
endif

define do_install
install:: $(1) ; $$(INSTALL) -d $$($(2)DIR) ; $$(INSTALL) $$($(or $(3),$(2))IFLAGS) $(1) $$($(2)DIR)/$(1)
endef

define do_archive
$(1): $(2) ; $$(AR) $$(ARFLAGS) $(1) $(2)
endef



#include "gmake_generator.h"
#include "base_generator.h"
#include "compound_artifact.h"
#include "location.h"

namespace butter {

const basic_style gmake_generator::style( "#", "", "##END:", "", "##START:", "", "make", &butter::gmake_generator::create );

//The default leaf filename for the current style

const QString gmake_generator::build_file_name("makefile");

//The default leaf filename for the current style

const QString gmake_generator::build_file_sysname("makefile");

//This is the default contents of a the rules file (M_sys.mk).
//If a document artifact with name 'M_sys.mak' is not present when 
//%butter is executed with \@style='standard' then one will be created
//using this string.

const char * gmake_generator::default_rules_sys[] { "#\n"
, "# M_sys.mk\n"
, "#\n"
, "##################################\n"
, "# Settings for current environment\n"
, "##################################\n"
, "\n"
, "OS ?= unix\n"
, "ifeq ($(origin CC),default)\n"
, "CC = gcc\n"
, "endif\n"
, "VARIANT ?= DEBUG\n"
, "\n"
, "# Preprocessor flags (defines and include directories)\n"
, "#  CPPFLAGS\n"
, "# C language flags\n"
, "#  CCFLAGS\n"
, "# C++ language flags\n"
, "#  CCCFLAGS\n"
, "# Fortran language flags\n"
, "#  FORTRANFLAGS\n"
, "# Linker flags\n"
, "#  LDFLAGS\n"
, "\n"
, "# Optimisation/debugging flags\n"
, "# are added depending on VARIANT\n"
, "\n"
, "##########################\n"
, "## Include compiler and OS\n"
, "##########################\n"
, "include $(ROOTDIR)$(or $(SLASH),/)M_$(OS).mk\n"
, "include $(ROOTDIR)$(or $(SLASH),/)M_$(CC).mk\n"
, "\n"
, "##############\n"
, "## Unify Vars\n"
, "##############\n"
, "CFLAGS = $(CCFLAGS)\n"
, "CXXFLAGS = $(CCCFLAGS)\n"
, "FFLAGS = $(FORTRANFLAGS)\n"
, "FC = $(FORTRAN)\n"
, "\n"
, "#############################################\n"
, "##  Define standard directory descent targets\n"
, "#############################################\n"
, "export\n"
, "unexport dirs\n"
, "\n"
, "all:: ; $(call doit,all,$(dirs))\n"
, "\n"
, "install:: ; $(call doit,install,$(dirs))\n"
, "\n"
, "distclean: doclean\n"
, "\t$(call doit,distclean,$(dirs))\n"
, "\t$(RM) core core.* $(TARGETS) *~ $(if $(sufdep),*$(sufdep))\n"
, "\n"
, "clean: doclean\n"
, "\t$(call doit,clean,$(dirs))\n"
, "\n"
, "doclean: \n"
, "\t$(if $(USEROBJ),$(RM) $(USEROBJ))\n"
, "\n"
, "################################################\n"
, "##  Example link library setup with alternatives\n"
, "################################################\n"
, "##  \n"
, "##  # Make this a one time only definition\n"
, "##  \n"
, "##  ifndef USE_XML_MK\n"
, "##  USE_XML_MAK:=1\n"
, "##  \n"
, "##  ifdef ($(XMLLIB))\n"
, "##  ifeq ($(XMLLIB),EXPAT)\n"
, "##  CPPFLAGS+=-DEXPAT\n"
, "##  LDFLAGS+=-lexpat\n"
, "##  endif\n"
, "##  \n"
, "##  ifeq ($(XMLLIB),XML2)\n"
, "##  CPPFLAGS+=-DXML2 `xml2-config --cflags`\n"
, "##  LDFLAGS+=`xml2-config --libs`\n"
, "##  endif\n"
, "##  \n"
, "##  endif # XMLLIB\n"
, "##  \n"
, "##  endif # USE_XML_MK\n"
, "\n"
, 0 }
;

//This is the default contents of the rules file (M_cl.mak) for
//the microsoft compiler. If a 
//document artifact with name 'M_cl.mk' is not present when 
//%butter is executed with \@style='standard' then one will be created
//using this string.
//

const char * gmake_generator::default_rules_cl[] { "##########################################\n"
, "##  Definitions for Microsoft VC compiler\n"
, "#########################################\n"
, "ifndef M_CL_MK\n"
, "M_CL_MAK:=1\n"
, "\n"
, "CCC:=cl\n"
, "CXX:=cl\n"
, "AR:=link\n"
, "ARFLAGS:= /LIB\n"
, "FORTRAN:=fc\n"
, "CPPFLAGS?=\n"
, "CCFLAGS+=/Za /nologo /errorReport:none\n"
, "CCCFLAGS+=/GR /Gm- /EHsc /Za /Zc:forScope /nologo /errorReport:none\n"
, "\n"
, "ifeq ($(VARIANT),DEBUG)\n"
, "OPTFLAGS+=/Od /Zi\n"
, "LDFLAGS+=/MTd\n"
, "else\n"
, "OPTFLAGS+=/O2\n"
, "LDFLAGS+=/MT\n"
, "endif\n"
, "\n"
, "CCFLAGS+=$(OPTFLAGS)\n"
, "CCCFLAGS+=$(OPTFLAGS)\n"
, "FORTRANFLAGS?=\n"
, "LDFLAGS+=$(OPTFLAGS)\n"
, "\n"
, "FORTRANLIBS:=\n"
, "OPENMP:=/openmp\n"
, "\n"
, "endif # end of once-only\n"
, "\n"
, "#  commands to execute (built-in):\n"
, "COMPILE.c = $(CC) $(subst -,/,$(subst -L,/LIBPATH ,$(patsubst -l%,%.lib,$(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH)))) /c /Fo\n"
, "COMPILE.cc = $(CXX) $(subst -,/,$(subst -L,/LIBPATH ,$(patsubst -l%,%.lib,$(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH)))) /c /Fo\n"
, "define _makeobj_\n"
, "%$$(sufobj): %$(1) ; $$(COMPILE$(1))$$@ $$<\n"
, "endef\n"
, "\n"
, "$(foreach suff,.c .cc .cpp .cxx,$(eval $(call _makeobj_,$(suff))))\n"
, "\n"
, "# Commands to create dummy make dependency files.\n"
, "# (Or use depend generator such as X11 makedepend\n"
, "# or Digital Mars makedep or makedep.py)\n"
, "# %$(sufdep): %$(1)\n"
, "# \tstart makedep.py $$@ $$< $(CC) /showInclude /Zs /nologo $(CPPFLAGS) $(CXXFLAGS)\n"
, "define _makedep_\n"
, "%$(sufdep): %$(1) ; @echo # Dummy file  $$@\n"
, "endef\n"
, "\n"
, "$(foreach suff,.c .C .cc .cpp .cxx,$(eval $(call _makedep_,$(suff))))\n"
, "\n"
, "_makeobj_:=\n"
, "_makedep_:=\n"
, "\n"
, "define do_link_exe\n"
, "$(1) : $(2); $$(LINK.cpp) $$(OUTPUT_OPTIONS) /Fe$(1) $(2)\n"
, "endef\n\n"
, "define do_link_shr\n"
, "$(1) : CCFLAGS+=$(SHRFLAGS)\n"
, "$(1) : CCCFLAGS+=$(SHRFLAGS)\n"
, "$(1) : $(2); $$(LINK.cpp) $$(OUTPUT_OPTIONS) /dll /Fe$(1) $(2)\n"
, "endef\n\n"
, "define do_archive\n"
, "$(1): $(2) ; $$(AR) $$(ARFLAGS) /OUT:$(1) $(2)\n"
, "endef\n\n"
, 0 }
;

//This is the default contents of the rules file (M_gcc.mk) for
//the GNU compiler collection. If a 
//document artifact with name 'M_gcc.mk' is not present when 
//%butter is executed with \@style='standard' then one will be created
//using this string.

const char * gmake_generator::default_rules_gcc[] { "####################################\n"
, "##  Definitions for GNU g++ compiler\n"
, "####################################\n"
, "ifndef M_GCC_MK\n"
, "M_GCC_MK:=1\n"
, "##\n"
, "## Define compiler variables once only.\n"
, "## \n"
, "CCC?=g++\n"
, "FORTRAN?=gfortran\n"
, "CPPFLAGS?=\n"
, "CCFLAGS+=-Wall -std=c99\n"
, "CCCFLAGS+=-Wall -std=c++98\n"
, "\n"
, "ifeq ($(VARIANT),DEBUG)\n"
, "OPTFLAGS+=-O0 -DDEBUG=1\n"
, "CCFLAGS+=-pedantic\n"
, "CCCFLAGS+=-Weffc++\n"
, "else\n"
, "OPTFLAGS+=-O2 -march=native -DDEBUG=0\n"
, "endif\n"
, "\n"
, "CCFLAGS+=$(OPTFLAGS)\n"
, "CCCFLAGS+=$(OPTFLAGS)\n"
, "FORTRANFLAGS+=$(OPTFLAGS)\n"
, "LDFLAGS?=\n"
, "SHRFLAGS?=-fpic\n"
, "\n"
, "FORTRANLIBS:=-lgfortran\n"
, "OPENMP:=-fopenmp\n"
, "\n"
, "endif\n"
, "# END ONCE-ONLY\n"
, "\n"
, "define _makedep_\n"
, "%$(sufdep): %$(1)\n"
, "\tset -e; $$(CCC) -MM $$(CPPFLAGS) $$< \\\n"
, "\t| sed 's/\\($$*\\)$$(sufobj)[ :]*/\\1$$(sufobj) $$@ : /g' > $$@;\\\n"
, "\t[ -s $$@ ] || rm -f $$@\n"
, "endef\n"
, "\n"
, "$(foreach suff,.c .C .cc .cpp .cxx,$(eval $(call _makedep_,$(suff))))\n"
, "_makedep_:=\n"
, "\n"
, "define do_link_exe\n"
, "$(1) : $(2); $$(LINK.cpp) $$(OUTPUT_OPTIONS) -o $(1) -Wl,--start-group $(2) -Wl,--end-group\n"
, "endef\n"
, "define do_link_shr\n"
, "$(1) : CCFLAGS+=$(SHRFLAGS)\n"
, "$(1) : CCCFLAGS+=$(SHRFLAGS)\n"
, "$(1) : $(2); $$(LINK.cpp) $$(OUTPUT_OPTIONS) -shared -Wl,-soname,$(1) -o $(1) -Wl,--start-group $(2) -Wl,--end-group\n"
, "endef\n\n"
, 0 }
;

//This is the default contents of the rules file (M_unix.mak) for
//a UNiX/POSIX like operating system. If a 
//document artifact with name 'M_unix.mak' is not present when 
//%butter is executed with \@style='standard' then one will be created
//using this string.
//

const char * gmake_generator::default_rules_unix[] { " # Definition of for loop in local shell.\n"
, "define doit\n"
, "$(if $(2),STATUS=0 ; for subdir in $(2); do $(MAKE) -C $${subdir}  $(1) || STATUS=$$? ; done ; exit $${STATUS})\n"
, "endef\n"
, "\n"
, "# MAKE MOST VARIABLES ONCE-ONLY\n"
, "ifndef M_UNIX_MK\n"
, "M_UNIX_MK:=1\n"
, "#############################\n"
, "##  Default install locations\n"
, "#############################\n"
, "PREFIX=$(ROOTDIR)/installdir\n"
, "BINDIR?=$(PREFIX)/bin\n"
, "LIBDIR?=$(PREFIX)/lib\n"
, "INCLUDEDIR?=$(PREFIX)/include\n"
, "DATADIR?=$(PREFIX)/share\n"
, "DOCDIR?=$(DATADIR)/doc\n"
, "MANDIR?=$(DATADIR)/man1\n"
, "HTMLDIR?=$(DATADIR)/html\n"
, "\n# Flags for the install targets.\n"
, "BINIFLAGS?=-m 755\n"
, "FILEIFLAGS?=-m 644\n"
, "LIBIFLAGS?=-m 644 \n"
, "MANIFLAGS?=-m 644\n"
, "INSTALL?=install\n"
, "\n# Standard suffixes\n"
, "sufobj:=.o\n"
, "sufexe:=\n"
, "suflib:=.a\n"
, "sufshr:=.so\n"
, "sufdep:=.d\n"
, "\n# Path separator\n"
, "SLASH?=/\n"
, "\n# END OF ONCE-ONLY.\n"
, "endif\n\n"
, "define do_install\n"
, "install:: $(1) ; $$(INSTALL) -d $$($(2)DIR) ; $$(INSTALL) $$($(or $(3),$(2))IFLAGS) $(1) $$($(2)DIR)/$(1)\n"
, "endef\n\n"
, "define do_archive\n"
, "$(1): $(2) ; $$(AR) $$(ARFLAGS) $(1) $(2)\n"
, "endef\n\n"
, 0 }
;

//This is the default contents of the rules file (M_Windows_NT.mk) for
//a (post NT) Micrsoft Windows like operating system. If a 
//document artifact with name 'M_Windows_NT.mk' is not present when 
//%butter is executed with \@style='standard' then one will be created
//using this string.

const char * gmake_generator::default_rules_winnt[] { "# Definition of for loop in local shell.\n\n"
, "define doit\n"
, "IF \"$(2)\" NEQ \"\" FOR %%W IN ( $(2) ) DO \"$(subst /,\\,$(MAKE))\" -C %%W $(1)\n"
, "endef\n"
, "\n"
, "ifndef M_WINNT_MK\n"
, "M_WINNT_MK:=1\n"
, "#############################\n"
, "##  Default install locations\n"
, "#############################\n"
, "PREFIX?=$(ROOTDIR)\\installdir\n"
, "BINDIR?=$(PREFIX)\\bin\n"
, "DATADIR?=$(PREFIX)\\share\n"
, "DOCDIR?=$(DATADIR)\\doc\n"
, "HTMLDIR?=$(DATADIR)\\html\n"
, "INCLUDEDIR?=$(PREFIX)\\include\n"
, "LIBDIR?=$(PREFIX)\\lib\n"
, "MANDIR?=$(DATADIR)\\man1\n"
, "\n# Standard Suffixes\n"
, "sufobj:=.obj\n"
, "sufexe:=.exe\n"
, "sufdep:=.dep\n"
, "suflib:=.lib\n"
, "sufshr:=.dll\n"
, "# Programs\n"
, "MKDIR?=MD\n"
, "COPY?=COPY /Y /B /V\n"
, "RM:=-DEL /F\n"
, "\n# Path separator\n"
, "SLASH?=\\\n"
, "\n# END OF ONCE-ONLY.\n"
, "endif\n\n"
, "define do_install\n"
, "install:: $(1) ; IF NOT EXIST $$($(2)DIR) ( $$(MKDIR) $$($(2)DIR) )\n\t$$(COPY) $(1) $$($(2)DIR)\\$(1)\n"
, "endef\n\n"
, 0 }
;

//Null terminated list of the default_rules_[] attributes.
//This is a simple list of the default_rules_[...] attributes, listed
//in the same order as the rules_name attribute. It is only
//used to initialise the default_rules attribute. 

const char * * gmake_generator::default_rules_list[] { default_rules_sys, default_rules_cl, default_rules_gcc, default_rules_unix, default_rules_winnt, 0 };

//A list containing _all_ the default rules.
//This is assigned all the default rules concatenated together. The
//rules appear in the order specified by rules_name.
//\sa default_rules_list

const char * * gmake_generator::default_rules= combine(gmake_generator::default_rules_list);

//A space separated list of rule names.
//This style has multiple system buildfiles. This variable contains a space
//separated list of the names of these files.

const QString gmake_generator::rules_name("M_sys.mk M_cl.mk M_gcc.mk M_unix.mk M_Windows_NT.mk");

//Create a generator from the top-level a_project
//
//\pre a_project.parent = nul
gmake_generator::gmake_generator()
: lib_set_ ()
, qualified_target_name_ ()
{
}

//Create bjam generator object.s
std::unique_ptr< base_generator > gmake_generator::create()
{
  std::unique_ptr< base_generator > Result (new gmake_generator);
  return Result;
}

//** This method a library association to the current target entry for a_target.
//
//Responsibilites
//- Properties
// - associated includes, ldflags, cflags
void gmake_generator::assoc_library() {
}

// This method sets up object for creating a new target entry for a_target.
//
//Responsibilites
//- Properties
// - doc/source: includes, ldflags, cflags
// - compiler
//- Other
// - set compilation for individual 
//
//\cond XDOC
//<property style="standard" name="include" on="artifact" subtype="stereotype=source"> Add
//include directories to compilation of this source only.  Any include directories
//needed to compile dependents of this source should be set in this property on
//the target artifact that is associated to the source.</property>
//\endcond XDOC
void gmake_generator::assoc_source() {
}

//Write the Jamfile descent links for a_location to a_os
void gmake_generator::descendent_link() {
// Need to write the "M_sys" include line.
{
  QString tmp_;
  QTextOStream desc_os_( &tmp_ );
  {
    const int e_ = rules_name.find( ' ' );
    QString name_ ( e_ < 0 ? rules_name : rules_name.left( e_ ) );
    desc_os_ << "include " << ( pathcmp( "$(ROOTDIR)" ) / name_ ).path_localised() << "\n\n";
  }
  a_art.document().append_preamble_value( tmp_ );
}
// Keep parent dir definitions.
if ( NULL != a_loc.parent() )
{
  // Add us to base makefile
  QString link( "dirs += " );
  link.append( pathcmp( root_dir().create_relative( a_loc.full_path() ) ).path_localised() );
  link.append( "\n" );
  const QString label( a_loc.path().path() );
  if( ! a_sys.document().has_target( label ) )
  {
    a_sys.document().add_target( label, "", link );
  }
  else
  {
    a_sys.document().append_target_value( label, link );
  }
}


}

//** This method finalises the target entry for a_target.
void gmake_generator::end_target() {
}

//In this style external targets are not used, external library 
//data is written in local-targets directly where it is 
//referenced.
void gmake_generator::external_target() {
}

//Write extra details to the top-level build file. 
void gmake_generator::initialise() {
}

//** Create an install target.
//
//Responsibilites
//- Properties
// - install (library, executable and document)
void gmake_generator::install_target() {
}

//Process a_inc_list and a_flag_list into preprocessor
//and compiler flag sets.
void gmake_generator::process_flags() {
QTextOStream cppos_ (&a_cppflags);
QTextOStream flagos_ (&a_cflags);
if (! a_inc_list.isEmpty())
{
  for (const_token_iterator e1_, b1_(a_inc_list.simplifyWhiteSpace (), ' '); b1_ != e1_; ++b1_)
  {
    const QString item_(b1_->c_str ());
    BUTTER_CHECK (! item_.isEmpty ()
                  , "<p><em>Programming error:</em> token iterator returned an empty string</p>");
    if ('$' != item_[0] && QDir::isRelativePath (item_))
    {
      cppos_ << "-I" << (pathcmp("$(ROOTDIR)") / item_).path_localised () << " ";
    }
    else
    {
      cppos_ << "-I" << pathcmp(item_).path_localised () << " ";
    }
  }
}
if (! a_flag_list.isEmpty())
{
  for (const_token_iterator e1_, b1_(a_flag_list.simplifyWhiteSpace (), ' '); b1_ != e1_; ++b1_)
  {
    const QString item_(b1_->c_str ());
    BUTTER_CHECK (! item_.isEmpty ()
                  , "<p><em>Programming error:</em> token iterator returned an empty string</p>");
    if (item_.length() <= 2)
    {
      flagos_ << item_ << " ";
    }
    else if ('-' == item_[0])
    {
      if ('I' == item_[1])
      {
        cppos_ << "-I" << pathcmp(item_.mid(2)).path_localised () << " ";
      }
      else if ('D' == item_[1])
      {
        cppos_ << item_ << " ";
      }
      else
      {
        flagos_ << item_ << " ";
      }
    }
    else
    {
      if ('$' == item_[0] && (item_.contains("INC") || item_.contains("CPP") || item_.contains("HDR")))
      {
        cppos_ << item_ << " ";
      }
      else
      {
        flagos_ << item_ << " ";
      }
    }
  }
}

}

//Transform a space separated list of include dirs into a
//list with -I...
QString gmake_generator::process_hdrs()
{
QString Result;
{
  QTextOStream os_ (&Result);
  for (const_token_iterator e1_, b1_(a_inc_list.simplifyWhiteSpace (), ' '); b1_ != e1_; ++b1_)
  {
    const QString item_(b1_->c_str ());
    if ('$' != item_[0] && QDir::isRelativePath (item_))
    {
      os_ << "-I$(ROOTDIR)/" << item_ << " ";
    }
    else
    {
      os_ << "-I" << item_ << " ";
    }
  }
}
return Result;
}

//Scan a_item for any requirements, add these to a_req and
//return true if found.
bool gmake_generator::requirements()
{
}

//** This method sets up object for creating a new target entry for a_target.
//
//Responsibility
//- Properties
// - compiler
// - include, ldflags, cflags
// - buildfile
// - type (library)
//- Other
// - shared lib
// - static lib
// - executable
// - non-standard target
void gmake_generator::start_target() {
}


} // namespace butter

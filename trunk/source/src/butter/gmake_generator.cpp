/**
 * Source for GNU make generator
 */
#include "butter/gmake_generator.h"
#include "bouml/UmlArtifact.h"
#include "butter/compound_artifact.h"
#include "butter/location.h"
#include "bouml/UmlItem.h"

// Manual source includes
#include "butter/config.h"
// -
namespace butter {

const basic_style gmake_generator::style( "#", "", "##END:", "", "##START:", "", "make", &butter::gmake_generator::create );

/**
 * The default leaf filename for the current style
 */
const QString gmake_generator::build_file_name("makefile");

/**
 * The default leaf filename for the current style
 */
const QString gmake_generator::build_file_sysname("makefile");

/**
 * This is the default contents of a the rules file (M_sys.mk).
 * If a document artifact with name 'M_sys.mak' is not present when 
 * %butter is executed with \@style='standard' then one will be created
 * using this string.
 */
const char * gmake_generator::default_rules_sys[] = { "#\n"
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

/**
 * This is the default contents of the rules file (M_cl.mak) for
 * the microsoft compiler. If a 
 * document artifact with name 'M_cl.mk' is not present when 
 * %butter is executed with \@style='standard' then one will be created
 * using this string.
 * 
 */
const char * gmake_generator::default_rules_cl[] = { "##########################################\n"
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

/**
 * This is the default contents of the rules file (M_gcc.mk) for
 * the GNU compiler collection. If a 
 * document artifact with name 'M_gcc.mk' is not present when 
 * %butter is executed with \@style='standard' then one will be created
 * using this string.
 */
const char * gmake_generator::default_rules_gcc[] = { "####################################\n"
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

/**
 * This is the default contents of the rules file (M_unix.mak) for
 * a UNiX/POSIX like operating system. If a 
 * document artifact with name 'M_unix.mak' is not present when 
 * %butter is executed with \@style='standard' then one will be created
 * using this string.
 * 
 */
const char * gmake_generator::default_rules_unix[] = { " # Definition of for loop in local shell.\n"
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
, "LIBDIR?=$(PREFIX)/bin\n"
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

/**
 * This is the default contents of the rules file (M_Windows_NT.mk) for
 * a (post NT) Micrsoft Windows like operating system. If a 
 * document artifact with name 'M_Windows_NT.mk' is not present when 
 * %butter is executed with \@style='standard' then one will be created
 * using this string.
 */
const char * gmake_generator::default_rules_winnt[] = { "# Definition of for loop in local shell.\n\n"
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

/**
 * Null terminated list of the default_rules_[] attributes.
 * This is a simple list of the default_rules_[...] attributes, listed
 * in the same order as the rules_name attribute. It is only
 * used to initialise the default_rules attribute. 
 */
const char ** gmake_generator::default_rules_list[] = { default_rules_sys, default_rules_cl, default_rules_gcc, default_rules_unix, default_rules_winnt, 0 };

static char const** combine(char const*** list )
{
  int size_ = 0;
  for (const char *** y = list ; 0 != *y; ++y, ++size_ )
    for (const char ** x = *y; 0 != *x; ++x, ++size_) ;
  char const** result = new char const*[size_];
  char const** cur = result;
  size_ = 0;
  for (char const*** y = &list[0] ; 0 != *y; ++y, ++cur, ++size_)
  {
    for (char const** x = *y; 0 != *x; ++x, ++cur, ++size_)
    {
      *cur = *x;
    }
    *cur = 0;
  }
  return result;
}
/**
 * A list containing _all_ the default rules.
 * This is assigned all the default rules concatenated together. The
 * rules appear in the order specified by rules_name.
 * \sa default_rules_list
 */
const char ** gmake_generator::default_rules = combine(gmake_generator::default_rules_list);

/**
 * A space separated list of rule names.
 * This style has multiple system buildfiles. This variable contains a space
 * separated list of the names of these files.
 */
const QString gmake_generator::rules_name("M_sys.mk M_cl.mk M_gcc.mk M_unix.mk M_Windows_NT.mk");

gmake_generator::gmake_generator()
: lib_set_ ()
, qualified_target_name_ ()
{}

std::unique_ptr< base_generator > gmake_generator::create()

{
  std::unique_ptr< base_generator > Result (new gmake_generator);
  return Result;
}

void gmake_generator::assoc_library(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString & a_includes, QString & a_ldflags, QString & a_cflags) {
////////////////////////////////////
// Define the associations of target
//
// to sources and libraries
//
QString project_;
QString lib_body_;
if (a_target.property_value (butter_constants::butter_project_name, project_))
{
  //////////////////////////////////
  // Is an externally linked library
  //
/* {{BEGINDOC
<property name="include" source="artifact" type="library" subtypes="external" style="make">Add
include directory properties to associated target.</property>

<property name="flags" source="artifact" type="library" subtypes="external" style="make">Add
compilation flags to associated target.</property>

<property name="ldflags" source="artifact" type="library" subtypes="external" style="make">Add
linker flags to associated target.</property>
ENDDOC}} */
    find_hdr_link (a_target, a_includes, a_ldflags, a_cflags, this->style.name(), true);
}
else
{
  ///////////////////
  // Is local library
  //
  QTextOStream lib_os_ (&lib_body_); // Stream for dependent library data
  const QString lib_path_ (root_dir ().create_relative (a_target.package().src_path ()));
  QString library_type_;
  pathcmp lib_fname_ ("$(ROOTDIR)");
  if (! lib_path_.isEmpty ())
  {
    lib_fname_ = lib_fname_ / lib_path_;
  }
  if (a_target.property_search (butter_constants::butter_lib_type_name, library_type_)
      && butter_constants::shared_value == library_type_)
  {
    //////////////////////
    // Is a shared library
    //
    const QString lib_name_ (a_target.name () + "$(sufshr)");
    lib_fname_ = lib_fname_ / lib_name_;
    // Add to linker information
    merge_string_list (a_ldflags, lib_fname_.path_localised ());
  }
  else
  {
    //////////////////////
    // Is a static library
    //
    const QString lib_name_ (a_target.name () + "$(suflib)");
    lib_fname_ = lib_fname_ / lib_name_;
    // Add to object list
    QString obj_body_;
    {
      QTextOStream obj_os_ (&obj_body_);
      obj_os_ << this->target_NAME () << "_OBJ+=" << lib_fname_.path_localised () << "\n";
    }
    this->individual_obj_.append (obj_body_);
  }
  //////////////////////
/* {{BEGINDOC
<property name="ldflags" source="artifact" type="source" style="make">Add
linker flags transitively to targets associated to the static or shared library
this source is associated with.</property>
ENDDOC}} */
  QVector < UmlArtifact > assoc_arts_ (const_cast< UmlArtifact& >(a_target).associatedArtifacts ());
  for (unsigned int i_ = 0; i_ < assoc_arts_.size (); ++i_)
  {
    const QString stereotype_ (assoc_arts_[i_]->stereotype ().data ());
    if (stereotype_ == butter_constants::library_stereotype
        || stereotype_ == butter_constants::source_stereotype)
    {
      QString ldflags_;
      if (assoc_arts_[i_]->property_value (butter_constants::butter_ldflags_name, ldflags_))
      {
        merge_string_list (a_ldflags, ldflags_);
      }
    }
  }
  lib_os_ << lib_fname_.path_localised () << " ";
  // Both static/shared add include and link flags, ignore cflags
/* {{BEGINDOC
<property name="ldflags" source="artifact" type="library" subtypes="shared static" style="make">
Add linker flags to targets associated to this library.</property>
<property name="include" source="artifact" type="library" subtypes="shared static" style="make">Add
include directories to targets associated to the library.</property>
ENDDOC}} */
  QString ignored_cflags_;
  find_hdr_link (a_target, a_includes, a_ldflags
      , ignored_cflags_, this->style.name(), true);
}
this->lib_set_.append (lib_body_);
}

void gmake_generator::assoc_source(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_filename, QString a_basename, QString a_src_inc, QString a_src_flags, bool a_isdoc) {
////////////////////////////////////
// Define the associations of target
//
// to sources and documents
//
/* \cond xdoc
<xdoc:artifact style="make">Filename of associated artifact is added to target's
source variable</xdoc:artifact>
\endcond xdoc */
a_os << " \\\n\t" << a_filename;

QString source_extra_;
{
  QTextOStream source_extra_os_ (&source_extra_);
  if (a_isdoc)
  {
    /* \cond xdoc
<xdoc:artifact stereotype="document" style="make">Filename with suffix replaced with
the object suffix is added to target's object variable.</xdoc:artifact>
    \endcond xdoc */
    source_extra_os_ << this->target_NAME () << "_OBJ+=" << a_basename << "$(sufobj)\n";
  }
  /* \cond xdoc
<xdoc:section part-of="property/include/make">
   <dl><dt>artifact/stereotype=source</dt><dd>Add
include directories to compilation of this source only.  Any include directories
needed to compile dependents of this source should be set in this property on
the target artifact that is associated to the source.</dd></dl></xdoc:section>

<xdoc:section part-of="property/flags/make">
  <dl><dt>artifact/stereotype=source</dt><dd>Add to
compilation of this source only. Any flags needed to compile dependents of this
source should be set in this property on the target artifact that is associated
to the source.</dd></dl></xdoc:section>

<xdoc:section part-of="property/compiler/make">
  <dl><dt>artifact/stereotype=source</dt><dd>When <xdoc:xref name="property/flags">flags</xdoc:xref>
   is set, [compiler]FLAGS is adjusted instead of CCCFLAGS</dd></dl></xdoc:section>
  \endcond xdoc */
  if (! a_src_inc.isEmpty () || ! a_src_flags.isEmpty ())
  {
    QString cppflags_, compflags_;
    this->process_flags (a_src_inc, a_src_flags, cppflags_, compflags_);
    QString comp_ ("CCC");
    BUTTER_ALWAYS(a_target.property_value (butter_constants::butter_compiler_name, comp_)
        || ! a_isdoc
                  , ("<p><b>Error:</b> Missing '" + butter_constants::butter_compiler_name
                                  + "' property on " + QString (a_filename)).utf8 ().data ());
    if (! cppflags_.isEmpty ())
    {
      source_extra_os_ << a_basename << "$(sufobj) " << a_basename << "$(sufdep) : CPPFLAGS+=" << cppflags_ << "\n";
    }
    if (! compflags_.isEmpty ())
    {
      source_extra_os_ << a_basename << "$(sufobj) : " << comp_ << "FLAGS+=" << compflags_ << "\n";
    }
    source_extra_os_ << "\n";
  }
}
if (! source_extra_.isEmpty ())
{
  this->individual_obj_.append ("\n");
  this->individual_obj_.append (source_extra_);
}
}

void gmake_generator::descendent_link(compound_artifact & a_art, compound_artifact & a_sys, const location & a_loc) 
{
// Need to write the "M_sys" include line.
QString tmp_;
QTextOStream desc_os_ (&tmp_);
{
  const int e_ = rules_name.find(' ');
  QString name_ (e_ < 0 ? rules_name : rules_name.left(e_));
  desc_os_ << "include " << (pathcmp("$(ROOTDIR)") / name_).path_localised () << "\n\n";
}
// Keep parent dir definitions.
if (NULL != a_loc.parent ())
{
  // Add us to base makefile
  QString & link_ = a_sys.target (a_loc.path ().path ()).second;
  link_.append ("dirs += ");
  link_.append (pathcmp(root_dir ().create_relative (a_loc.full_path ())).path_localised ());
  link_.append ("\n");
}
a_art.preamble.second.append (tmp_);


}

void gmake_generator::end_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_include, QString a_ldflags, QString a_cflags, QString a_compiler, base_generator::target_type a_type) {
////////////////////////////////
// Finalise target build instructions
//
a_os << "\n";
if (a_type == other)
{
  ////////////////
  // Handle "other" library types specially.
  QString other_type_( this->other_target_type_.upper() );
  other_type_.replace( QChar{'-'}, QChar{'_'} );
  QString cppflags_, compflags_;
  this->process_flags(a_include, a_cflags, cppflags_, compflags_);
  if (! compflags_.isEmpty ())
  {
    a_os << this->qualified_target_name_ << ": " << other_type_ << "FLAGS+=" << compflags_ << "\n";
  }
  if (! cppflags_.isEmpty ())
  {
    a_os << this->qualified_target_name_ << ": " << other_type_ << "FLAGS+=" << cppflags_ << "\n";
  }
  if (! a_ldflags.isEmpty ())
  {
    a_os << this->qualified_target_name_ << ": " << other_type_ << "FLAGS+=" << a_ldflags << "\n";
  }
  a_os << this->qualified_target_name_ << " : $(" << this->target_NAME () << "SRC) " << this->lib_set_ << "\n"
        << "\t$(" << other_type_ << ") $(" << other_type_ << "FLAGS) " << this->qualified_target_name_
            << " $(" << this->target_NAME () << "SRC)\n\n";
}
else
{
  // Add dependency information
  a_os << target_NAME () << "_DINC := $(filter %$(sufdep), $(" << target_NAME ()
      << "SRC:." << CppSettings::sourceExtension () << "=$(sufdep)))"
      << " $(filter %$(sufdep), $(" << target_NAME () << "SRC:.c=$(sufdep)))\n";
  // Create linker and compiler flagsets
  if (! a_ldflags.isEmpty ())
  {
    a_os << this->qualified_target_name_ << ": LDFLAGS+=";
    if (! a_ldflags.isEmpty ())
    {
      a_os << " " << a_ldflags;
    }
    a_os << "\n";
  }
  QString cppflags_, compflags_;
  this->process_flags(a_include, a_cflags, cppflags_, compflags_);
  if (! compflags_.isEmpty ())
  {
    a_os << this->qualified_target_name_ << ": CCCFLAGS+=" << compflags_ << "\n";
    a_os << this->qualified_target_name_ << ": CCFLAGS+=" << compflags_ << "\n";
  }
  if (! cppflags_.isEmpty ())
  {
    // Set preprocessor flags on target and dependency files
    a_os << this->qualified_target_name_ << " $("
        << target_NAME () << "_DINC) : CPPFLAGS+=" << cppflags_ << "\n";
  }
  // Add any object definitions.
  if (! this->individual_obj_.isEmpty ())
  {
    a_os << this->individual_obj_ << "\n";
  }
  a_os << target_NAME () << "_OBJ+="
      << "$(filter %$(sufobj), $(" << target_NAME ()
      << "SRC:." << CppSettings::sourceExtension ()
      << "=$(sufobj)))\n"
      << "USEROBJ+=$(" << target_NAME () << "_OBJ)\n";
  a_os << this->qualified_target_name_
          << " : $(" << this->target_NAME () << "_SRC) $("
                      << this->target_NAME () << "_DINC)\n";
  // Include dependency files.
  a_os << "-include $(" << target_NAME () << "_DINC)\n";

  // Finally add target build definition
  switch (a_type)
  {
  case executable:
    {
      a_os << "$(eval $(call do_link_exe," << this->qualified_target_name_ << ",$(" << this->target_NAME () << "_OBJ)))\n";
      break;
    }
  case shared_library:
    {
      a_os << "$(eval $(call do_link_shr," << this->qualified_target_name_
          << ",$(" << this->target_NAME () << "_OBJ),$(or $(" << this->target_NAME () <<"_VERSION),0)))\n";
      break;
    }
    break;
  case static_library:
    {
      a_os << "$(eval $(call do_archive," << this->qualified_target_name_ << ",$(" << this->target_NAME () << "_OBJ)))\n";
      break;
    }
    break;
  case other: // Can not get here.
    break;
  }
}

a_os << "\nall :: " << this->qualified_target_name_ << "\n";
a_os << "TARGETS+=" << this->qualified_target_name_ << "\n\n";
}

void gmake_generator::initialise(location & a_base, const ::UmlItem & a_project, compound_artifact & a_sys) 
{
// The include dir is the difference between the current fullpath and the
// top-projects hdrDir
QString init_text_;
{
  QTextOStream init_os_ (&init_text_);
  init_os_ << "ROOTDIR := " << root_dir ().path_localised () << "\n"
         << "OUTPUTDIR := " << root_dir ().path_localised () << "\n";
  {
    const int e_ = rules_name.find(' ');
    QString name_ (e_ < 0 ? rules_name : rules_name.left(e_));
    init_os_ << "include " << (pathcmp("$(ROOTDIR)") / name_).path_localised () << "\n\n";
  }
  const pathcmp base_include_ (dynamic_cast< const UmlPackage& >(a_project).hdr_path ());

  // Create list of include directories
  QString inc_;
  a_project.property_value (butter_constants::butter_include_name, inc_);
  if (!root_dir ().equality (base_include_))
  {
    inc_.append(" " + root_dir ().create_relative (base_include_));
  }

  // Get list of flags
  QString flags_;
  a_project.property_search (butter_constants::butter_flags_name, flags_);

  // Process flags into preprocessor and compiler sets
  process_flags (inc_, flags_, inc_, flags_);
  if (!inc_.isEmpty())
  {
    init_os_ << "CPPFLAGS+=" << inc_ << "\n";
  }
  if (!flags_.isEmpty())
  {
    init_os_ << "CCFLAGS+=" << flags_ << "\n";
    init_os_ << "CCCFLAGS+=" << flags_ << "\n";
  }

  // Get linker flags
  QString linkflag_;
  if (a_project.property_search (butter_constants::butter_ldflags_name, linkflag_))
  {
    init_os_ << "LDFLAGS+=" << linkflag_ << "\n";
  }
}
a_sys.preamble.second = init_text_;

}

void gmake_generator::install_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_loc_var, base_generator::install_type a_type, bool a_isdoc) 
{
static const char * install_flag_[] =
{
  "BIN"
, "FILE"
, "LIB"
, "MAN"
};
QString target_name_;
if (a_isdoc)
{
  target_name_ = a_target.name ();
}
else
{
  target_name_ = this->qualified_target_name_;
}
// Remove trailing DIR
if (a_loc_var.right(3) == "DIR")
{
  a_loc_var = a_loc_var.left(a_loc_var.length() - 3);
}
a_os << "$(eval $(call do_install," << target_name_
     << "," << a_loc_var;
if (a_loc_var != install_flag_ [a_type])
{
   a_os << "," << install_flag_ [a_type];
}
a_os << "))\n\n";

}

void gmake_generator::process_flags(QString a_inc_list, QString a_flag_list, QString & a_cppflags, QString & a_cflags) 
{
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

QString gmake_generator::process_hdrs(QString a_inc_list)
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

bool gmake_generator::requirements(const ::UmlItem & a_item, QString & a_reqs)
{
bool Result (false);
QString l_tmp;
QString l_section;
QTextOStream l_os (&l_section);
if (a_item.property_value (butter_constants::butter_include_name, l_tmp))
{
  Result = true;
  l_os << "USERHDRS := $(USERHDRS) " << process_hdrs (l_tmp) << "\n";
}
if (a_item.property_search (butter_constants::butter_ldflags_name, l_tmp))
{
  Result = true;
  l_os << "USERLDFLAGS := $(USERLDFLAGS) " << l_tmp << "\n";
}
if (a_item.property_search (butter_constants::butter_flags_name, l_tmp))
{
  Result = true;
  l_os << "USERFLAGS := $(USERFLAGS) " << l_tmp << "\n";
}
if (Result)
{
  a_reqs = l_section;
}
return Result;
}

void gmake_generator::start_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_build_file, QString a_compiler, base_generator::target_type a_type) {
/////////////////
// Initialise variables for the new target
this->qualified_target_name_ = a_target.name ();
this->target_NAME (a_target);
this->other_target_type_.truncate (0);    // An "other" target type.
this->lib_set_.truncate (0);              // Set of included libraries
this->individual_obj_.truncate (0);       // Set of sources that need to be individually compiled
///////////////////////////////////////////
// Define the characteristics of the target
//
// a_target must be executable or library stereotype!
//
switch (a_type)
{
case executable:
  {
    this->qualified_target_name_.append ("$(sufexe)");
  }
  break;
case static_library:
  this->qualified_target_name_.append ("$(suflib)");
  break;
case shared_library:
  this->qualified_target_name_.append ("$(sufshr)");
  break;
case other:
  a_target.property_value (butter_constants::butter_other_name, this->other_target_type_);
  break;
}

// Handle target specific include file for library types only
if (a_type != executable)
{
  if (! a_build_file.isEmpty ())
  {
    a_os << "-include " << (pathcmp("$(ROOTDIR)")
        / a_build_file).path_localised() << ".mak\n\n";
    a_os << "-include " << (pathcmp("$(ROOTDIR)")
        / a_build_file).path_localised() << ".make\n\n";
  }
}

//////////////////
// Start the source file associations
a_os << this->target_NAME () << "SRC := ";
}


} // namespace butter

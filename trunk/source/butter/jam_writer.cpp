/**
 * Source file for Jam writer
 */
#include "butter/jam_writer.h"
#include "butter/item_traits.h"
#include "bouml/UmlArtifact.h"
#include "butter/compound_artifact.h"
#include "butter/location.h"
#include "bouml/UmlItem.h"

// Manual source includes
#include <qstringlist.h>
#include <qregexp.h>
// -
namespace butter {

const QString jam_writer::build_file_name("Jamfile");

const QString jam_writer::build_file_sysname("Jamfile");

const QString jam_writer::comment_string("#");

const QString jam_writer::end_phrase("##END:");

const QString jam_writer::start_phrase("##START:");

/**
 * The label for description 'sections' and the value of 
 * the style for this buildfile type.
 */
const QString jam_writer::section_name("standard");

/**
 * A template Jamrules.
 */
const char * jam_writer::default_rules[] = { "#\n"
, "# Default Jamrules\n"
, "#\n"
, "\nif $(UNIX)  { SUFSHR ?= .so ; }\n"
, "else if $(NT) { SUFSHR ?= .dll ; }\n"
, "\n"
, "COMP ?= GNU ;\n"
, "VARIANT ?= DEBUG ;\n"
, "\n"
, "#############################\n"
, "##\n"
, "##  Default install locations\n"
, "##\n"
, "#############################\n"
, "VERSIONDIR = \"@@project@@-@@version@@\" ;\n"
, "PREFIX = installdir ;\n"
, "EXEC_PREFIX = $(PREFIX) ;\n"
, "BINDIR = $(EXEC_PREFIX)/bin ;\n"
, "SBINDIR = $(EXEC_PREFIX)/sbin ;\n"
, "LIBEXECDIR = $(EXEC_PREFIX)/libexec ;\n"
, "LIBDIR = $(EXEC_PREFIX)/lib ;\n"
, "DATAROOTDIR = $(PREFIX)/share ;\n"
, "DATADIR = $(PREFIX)/share ;\n"
, "SYSCONFDIR = /etc ;\n"
, "SHAREDSTATEDIR = $(PREFIX)/com ;\n"
, "LOCALSTATEDIR = $(PREFIX)/var ;\n"
, "INCLUDEDIR = $(PREFIX)/include ;\n"
, "LOCALEDIR = $(DATAROOTDIR)/locale ;\n"
, "DOCDIR = $(DATAROOTDIR)/doc/$(VERSIONDIR) ;\n"
, "INFODIR = $(DATAROOTDIR)/info ;\n"
, "MANDIR = $(DATAROOTDIR)/man ;\n"
, "HTMLDIR = $(DOCDIR) ;\n"
, "DVIDIR = $(DOCDIR) ;\n"
, "PDFDIR = $(DOCDIR) ;\n"
, "PSDIR = $(DOCDIR) ;\n"
, "MAN1DIR = $(MANDIR)/man1 ;\n"
, "MAN2DIR = $(MANDIR)/man2 ;\n"
, "MAN3DIR = $(MANDIR)/man3 ;\n"
, "MAN4DIR = $(MANDIR)/man4 ;\n"
, "MAN5DIR = $(MANDIR)/man5 ;\n"
, "MAN6DIR = $(MANDIR)/man6 ;\n"
, "MAN7DIR = $(MANDIR)/man7 ;\n"
, "MAN8DIR = $(MANDIR)/man8 ;\n"
, "MANNDIR = $(MANDIR)/mann ;\n"
, "\n"
, "######################################################\n"
, "##\n"
, "##  Example definitions for compiler suite (TOOLSETS)\n"
, "## \n"
, "######################################################\n"
, "switch $(COMP)\n"
, "{\n"
, "case GNU :\n"
, "CC = gcc ;\n"
, "C++ = g++ ;\n"
, "FORTRAN = gfortran ;\n"
, "CCSTD = -std=c99 ;\n"
, "C++STD = -std=c++98 ;\n"
, "CCCPP = ;\n"
, "C++CPP = ;\n"
, "SHRFLAGS = -fpic ;\n"
, "ARCH = -march=native ;\n"
, "OPTIMCOMP = -O2 $(ARCH) ;\n"
, "DEBUGCOMP = -O0 -ggdb -DDEBUG=1 ;\n"
, "CCCOMP = -Wall -pedantic $(CCSTD) $(CCCPP) ;\n"
, "C++COMP = -Wall -Weffc++ $(C++STD) $(C++CPP) ;\n"
, "FORTRANCOMP = -Wall -c ;\n"
, "LINKCOMP = ;\n"
, "OPENMP = -fopenmp ;\n"
, "FORTRANLIBS = -lgfortran ;\n"
, "}\n"
, "\n################################################\n"
, "##\n"
, "##  Define the \"standard\" GUI flags\n"
, "## \n"
, "################################################\n"
, "\n"
, "CCGUI = ;\n"
, "C++GUI = ;\n"
, "LINKGUI = ;\n"
, "\n"
, "switch $(GUILIB)\n"
, "{\n"
, "case GTKMM :\n"
, "CCGUI = `pkg-config gtkmm-2.4 --cflags` ;\n"
, "C++GUI = `pkg-config gtkmm-2.4 --cflags` ;\n"
, "LINKGUI = `pkg-config gtkmm-2.4 --libs` ;\n"
, "\n"
, "case MOTIF :\n"
, "CCGUI = -I/usr/include ;\n"
, "C++GUI = -I/usr/include ;\n"
, "LINKGUI = -L/usr/lib -lXm -lXp -lXpm -lXmu -lXt -lXext -lX11 ;\n"
, "\n"
, "case QT3 :\n"
, "CCGUI = -I$(QTDIR)/include ;\n"
, "C++GUI = -I$(QTDIR)/include ;\n"
, "LINKGUI = -L$(QTDIR)/lib -lqt-mt -lXext -lX11 -lm ;\n"
, "\n"
, "case * :\n"
, "CCGUI = ;\n"
, "C++GUI = ;\n"
, "LINKGUI = ;\n"
, "\n"
, "}\n"
, "\n"
, "#################################\n"
, "##\n"
, "##  Define the \"standard\" flags\n"
, "## \n"
, "#################################\n"
, "\n"
, "CCFLAGS += $(CCCOMP) $(CCGUI) ;\n"
, "C++FLAGS += $(C++COMP) $(C++GUI) ;\n"
, "LINKLIBS += $(LINKCOMP) $(LINKGUI) ;\n"
, "\n"
, "OPTIM = $(DEBUGCOMP) ;\n"
, "\n"
, "switch $(VARIANT)\n"
, "{\n"
, "case RELEASE :\n"
, "OPTIM = $(OPTIMCOMP) ;\n"
, "}\n"
, "\n"
, "FORTRANFLAGS += $(FORTRANCOMP) $(OPTIM) ;\n"
, "LINKFLAGS = $(OPTIM) ;\n"
, "LINK = $(C++) ;\n"
, "\n"
, "# Set variable for variant-specific build-dir handling\n"
, "BASE_LOCATE_TARGET = $(BUILDDIR)/$(VARIANT) ;\n"
, "ALL_LOCATE_TARGET = $(BASE_LOCATE_TARGET) ;\n"
, "\n"
, "# Local Variables:\n"
, "# mode: makefile\n"
, "# End: \n"
, 0 };

/**
 * The name of the jam rules document.
 */
const QString jam_writer::rules_name("Jamrules");

void jam_writer::assoc_library(const ::UmlArtifact & a_target, QTextStream & a_os, QString & a_includes, QString & a_ldflags, QString & a_cflags) 
{
QString project_;
if (a_target.property_value (item_traits::butter_project_name, project_))
{
  // Is an externally linked library, look only for flags.
/* {{BEGINDOC
<property name="include" source="artifact" type="library" subtypes="external" style="standard">Add
include directory properties to associated target.</property>
<property name="flags" source="artifact" type="library" subtypes="external" style="standard">Add
compilation flags to associated target.</property>
<property name="ldflags" source="artifact" type="library" subtypes="external" style="standard">Add
linker flags to associated target.</property>
ENDDOC}} */
  find_hdr_link (a_target, a_includes, a_ldflags, a_cflags, section_name, true);
}
else
{
  {
    QString defn_;
    {
      QTextOStream os_ (&defn_);
      const QString dir_line_
          = this->root_dir ().create_relative (a_target.package ().src_path ());

      os_ << " <" << pathcmp (dir_line_).path_convert ("!") << ">"
          << a_target.name () << "$(SUFLIB) \n\t";
    }
    this->lib_defn_.append (defn_);
  }
/*  QString libtype_; // Add LOC_TARGET_LDFLAGS variable for static libraries
  if (a_target.property_search (item_traits::butter_lib_type_name, libtype_)
      && libtype_ != item_traits::shared_value)*/
  {
/* {{BEGINDOC
<property name="ldflags" source="artifact" type="source" style="standard">Add
linker flags transitively to targets associated to the static library
this source is associated with.</property>
<property name="ldflags" source="artifact" type="library" subtypes="static" style="standard">
Add linker flags to targets associated to this library.</property>
<property name="include" source="artifact" type="library" subtypes="shared static" style="standard">Add
include directories to targets associated to the library.</property>
ENDDOC}} */
    QString defn_;
    {
      QTextOStream os_ (&defn_);
      os_ << " $(" << to_target_NAME (a_target) << "_LDFLAGS) ";
    }
    a_ldflags.append (defn_);
  }
  QString libinc_; // Add include variable for both static and shared libraries
  if (a_target.property_search (item_traits::butter_include_name, libinc_))
  {
    a_includes.append (libinc_);
  }
}

}

void jam_writer::assoc_source(const ::UmlArtifact & a_target, QTextStream & a_os, QString a_filename, QString a_basename, QString a_src_inc, QString a_src_flags, bool a_isdoc) 
{
///////////////////////////
// Description for individual objects (optional)
QString source_;
QTextOStream source_os_ (&source_);
if (! a_src_inc.isEmpty ())
{
  source_os_ << "ObjectHdrs " << this->grist_
              << a_filename << " : " << a_src_inc << " ;\n";
}
if (! a_src_flags.isEmpty ())
{
  if (a_isdoc)
  {
     QString comp_;
     BUTTER_ALWAYS(a_target.property_value (item_traits::butter_compiler_name, comp_)
                , ("<p><b>Error:</b> Missing '" + item_traits::butter_compiler_name
                 + "' property on " + a_target.package().filename () + "/" + a_filename).utf8 ().data ());
     if (comp_ == "CC")
     {
       source_os_ << "ObjectCcFlags " << this->grist_
              << a_filename << " : " << a_src_flags << " ;\n";
     }
     else
     {
       source_os_ << comp_ << "FLAGS on " << this->grist_
              << a_filename << " = $("  << comp_ << "FLAGS) "
              << a_src_flags << " ;\n";
     }
  }
  else
  {
    source_os_ << "ObjectC++Flags " << this->grist_
              << a_filename << " : " << a_src_flags << " ;\n";
  }
}
if (! source_.isEmpty ())
{
  this->individual_obj_.append (source_);
}

/////////////
// Add source to target
a_os << this->grist_ << a_filename << "\n\t";

}

void jam_writer::check_properties(bool a_is_source, const ::UmlArtifact & a_source, QTextStream & a_os) 
{

}

std::auto_ptr< build_writer > jam_writer::create()

{
  std::auto_ptr< build_writer > Result (new jam_writer);
  return Result;
}

void jam_writer::descendent_link(compound_artifact & a_art, compound_artifact & a_sys, const location & a_loc) {
// Need to write the TOP line.
if (NULL != a_loc.parent ())
{
  QString subdir_line_;  // The text of the preamble section (mainly SubDir)
  // Create subpath (eg. src/chemistry) ;
  const QString dir_line2_ = this->root_dir ().create_relative (a_loc.full_path ());
  // Create a space separated list of folders between here and TOP
  const QString dir_line_ = pathcmp (dir_line2_).path_convert (" ");
  // Reset grist
  this->grist_.truncate (0);
  {
    QTextOStream subdir_os_ (&subdir_line_);
    // Create LOCATE_TARGET
    subdir_os_ << "ALL_LOCATE_TARGET = $(BASE_LOCATE_TARGET)$(SLASH)"
          << pathcmp(dir_line2_).path_convert("$(SLASH)") << " ;\n\n"
          << "SubDir TOP " << dir_line_ << " ;\n\n";
  }
  a_art.preamble.second.insert (0, subdir_line_);
  // Add definition to top-level
  a_sys.close.second.append ("SubInclude TOP " + dir_line_ + " ;\n");
}
}

void jam_writer::end_target(const ::UmlArtifact & a_target, QTextStream & a_os, QString a_include, QString a_ldflags, QString a_cflags, QString a_compiler, build_writer::target_type a_type) 
{
////////////////
// End target source variable
a_os << ";\n\n";

switch (a_type)
{
case executable:
  {
    a_os << "Main " << this->qualified_target_name_ << " :\n\t$("
        << this->target_NAME () << "_SRC) ;\n\n";
    if (! a_compiler.isEmpty ())
    {
      a_os << "LINK on " << this->qualified_target_name_ << " = $(" <<  a_compiler << ") ;\n\n";
    }
    if (! a_ldflags.isEmpty ())
    {
      a_os << "LINKLIBS on " << this->qualified_target_name_ << " += " << a_ldflags << " ;\n";
    }
  }
  break;
case other:
  {
    a_target.property_value (item_traits::butter_other_name, this->other_target_type_);
    a_os << this->other_target_type_ << " " << this->qualified_target_name_  << " :\n\t$("
        << this->target_NAME () << "_SRC) ;\n\n";
    if (! a_ldflags.isEmpty ())
    {
      a_os << "LINKLIBS on " << this->qualified_target_name_ << " += " << a_ldflags << " ;\n";
    }
  }
  break;
case shared_library: // static and shared libraries (only static libraries supported!)
  {
    a_os << "Library " << this->qualified_target_name_  << " :\n\t$("
        << this->target_NAME () << "_SRC) ;\n\n";
/*    a_os << "# SharedCcFlagsOnSrc " << this->target_NAME () << "_SRC ;\n";
    a_os << "# SharedC++FlagsOnSrc " << this->target_NAME () << "_SRC ;\n";
    a_os << "# SharedLdFlagsOn " << this->qualified_target_name_ << " ;\n";*/
    if (! a_compiler.isEmpty ())
    {
      a_os << "LINK on " << this->qualified_target_name_ << " = $(" <<  a_compiler << ") ;\n\n";
    }
    a_os << this->target_NAME () << "_LDFLAGS = ";
    if (! a_ldflags.isEmpty ())
    {
      a_os << a_ldflags;
    }
    a_os << " ;\n";
/*    if (! a_ldflags.isEmpty ())
    {
      a_os << "LINKLIBS on " << this->qualified_target_name_ << " += " << a_ldflags << " ;\n";
    }*/
  }
  break;
default: // static libraries (only static libraries supported!)
  {
    a_os << "Library " << this->qualified_target_name_  << " :\n\t$("
        << this->target_NAME () << "_SRC) ;\n\n";
//    if (! a_compiler.isEmpty ())
//     {
//       a_os << "LINK on " << this->qualified_target_name_ << " = $(" <<  a_compiler << ") ;\n\n";
//     }
    a_os << this->target_NAME () << "_LDFLAGS = ";
    if (! a_ldflags.isEmpty ())
    {
      a_os << a_ldflags;
    }
    a_os << " ;\n";
  }
  break;
}

//////////////
// Set up includes, libs etc.
if (! this->lib_defn_.isEmpty ())
{
  a_os << "LinkLibraries " << this->qualified_target_name_ << " :\n\t" << this->lib_defn_ << ";\n";
}

if (a_type == other)
{
  // Use target type flag name on non-standard targets.
  if (! a_cflags.isEmpty ())
  {
    a_os << this->other_target_type_.upper () << "FLAGS on " << this->qualified_target_name_ << " += " << a_cflags << " ;\n";
  }
}
else
{
  // Compile flags are propagated to source files.
  if (! a_cflags.isEmpty ())
  {
    a_os << "ObjectCcFlags $(" << this->target_NAME () << "_SRC) : " << a_cflags << " ;\n";
    a_os << "ObjectC++Flags $(" << this->target_NAME () << "_SRC) : " << a_cflags << " ;\n";
  }
}
// Headers are set on source files for all targets
if (! a_include.isEmpty ())
{
  a_os << "ObjectHdrs $(" << this->target_NAME () << "_SRC) : " << a_include << " ;\n";
}
////////////////////
// Add any individually build objects
if (! this->individual_obj_.isEmpty ())
{
  a_os << this->individual_obj_ << "\n";
}

}

void jam_writer::initialise(location & a_base, const ::UmlItem & a_project, compound_artifact & a_sys) 
{
BUTTER_REQUIRE (NULL == a_base.parent (), "initialise can only be called on the top-most location");

QString content_;
{
  QTextOStream os_ (&content_);
  ////////////
  // Handle build-directory with no default.
  //
  // Note that this needs to be above "SubDir" to be available in Jamrules
  QString prop_value_;
  if (a_project.property_value (item_traits::butter_build_dir_name, prop_value_))
  {
    os_ << "BUILDDIR = " << pathcmp(prop_value_).path_localised () <<" ;\n";
  }
  else
  {
    os_ << "BUILDDIR = " << this->root_dir ().path_localised () <<" ;\n";
  }
  //////////////
  // Set this as top location and bring in Jamrules.
  os_ << "SubDir TOP ;\n";
  os_ << "OUTPUTDIR = $(BUILDDIR)$(SLASH)$(VARIANT) ;\n";
  //////////////
  // Handle user-defined project includes and flag properties
  ////////////
  QString property_value_;
  a_project.property_value (item_traits::butter_include_name, property_value_);
  // Also set an include dir if there is a  difference between the root_dir and the
  // projects hdrDir
  const pathcmp toplevel_include_ (dynamic_cast< const UmlPackage& >(a_project).hdr_path ());
  if (! toplevel_include_.path ().isEmpty ())
  {
    if (!this->root_dir ().equality (toplevel_include_))
    {
      if (this->root_dir ().has_subpath (toplevel_include_))
      {
        property_value_.append(" " + this->root_dir ().create_relative (toplevel_include_));
      }
      else
      {
        property_value_.append(" " + toplevel_include_.path ());
      }
    }
  }
  if (! property_value_.isEmpty ())
  {
    os_ << "HDRS += " << property_value_ << " ;\n";
  }
  if (a_project.property_search (item_traits::butter_ldflags_name, property_value_))
  {
    os_ << "LINKFLAGS += " << property_value_ << " ;\n";
  }
  if (a_project.property_search (item_traits::butter_flags_name, property_value_))
  {
    os_ << "C++FLAGS += " << property_value_ << " ;\n";
    os_ << "CCFLAGS += " << property_value_ << " ;\n";
  }
}
a_sys.preamble.second = content_;

}

void jam_writer::install_target(const ::UmlArtifact & a_target, QTextStream & a_os, QString a_loc_var, build_writer::install_type a_type, bool a_isdoc) 
{
static const char * install_cmd_[] =
{
  "InstallBin"
, "InstallFile"
, "InstallLib"
, "InstallMan"
};
a_os << install_cmd_ [a_type] << " $("  << a_loc_var << ") : ";
if (a_isdoc)
{
  a_os << this->grist_ << a_target.name ();
}
else
{
  a_os << this->qualified_target_name_;
}
a_os << " ;\n";
}

jam_writer::jam_writer()
: lib_defn_ ()
, grist_ ()
, qualified_target_name_ ()
{}

void jam_writer::start_target(const ::UmlArtifact & a_target, QTextStream & a_os, QString a_build_file, QString a_compiler, build_writer::target_type a_type) 
{
////////////////////////
// Initialise state for writing a new target entry
if (this->grist_.isEmpty ())
{
  const QString dir_line2_ = this->root_dir ().create_relative (a_target.package ().src_path ());
  this->grist_ = "<" + pathcmp (dir_line2_).path_convert ("!") + ">";
}
// Text for the associated targets
this->lib_defn_.truncate (0);
// Text for source specific flags
this->individual_obj_.truncate (0);

// Will become the full target name
const QString name_ (a_target.name ());
// Create qualified name for target.
this->qualified_target_name_ = this->grist_ + name_;
switch (a_type)
{
case executable:
  {
    this->qualified_target_name_.append ("$(SUFEXE)");
  }
  break;
case other:
  {
    if (! a_build_file.isEmpty ())
    {
      a_os << "include " << a_build_file << ".jam ;\n\n";
    }
  }
   break;
case shared_library: // shared libraries (only static libraries supported!)
  {
    this->qualified_target_name_.append ("$(SUFLIB)");
  }
  break;
default: // static libraries (only static libraries supported!)
  {
    this->qualified_target_name_.append ("$(SUFLIB)");
  }
  break;
}
this->target_NAME (a_target);

// Target label for non-standard targets
this->other_target_type_.truncate (0);

//////////////////////
// Start entry
a_os << this->target_NAME () << "_SRC =\n\t";

}


} // namespace butter

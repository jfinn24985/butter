/**
 * Source file for Jam generator
 */
#include "butter/jam_generator.h"
#include "bouml/UmlArtifact.h"
#include "butter/compound_artifact.h"
#include "butter/location.h"
#include "bouml/UmlItem.h"

// Manual source includes
#include <qregexp.h>
#include "butter/config.h"
// -
namespace butter {

const basic_style jam_generator::style( "#", "", "##END:", "", "##START:", "", "standard", &butter::jam_generator::create );

const QString jam_generator::build_file_name("Jamfile");

const QString jam_generator::build_file_sysname("Jamfile");

/**
 * A template Jamrules.
 */
const char * jam_generator::default_rules[]{ "#\n"
, "# Default Jamrules\n"
, "#\n"
, "\nif $(UNIX)  { SUFSHR ?= .so ; }\n"
, "else if $(NT) { SUFSHR ?= .dll ; }\n"
, "\n"
, "if $(JAM_TOOLSET) { TOOLSET ?= $(JAM_TOOLSET) ; }\n"
, "TOOLSET ?= GNU ;\n"
, "VARIANT ?= DEBUG ;\n"
, "\n"
, "#############################\n"
, "##\n"
, "##  Default install locations\n"
, "##\n"
, "#############################\n"
, "PREFIX = installdir ;\n"
, "BINDIR = $(PREFIX)/bin ;\n"
, "DATADIR = $(PREFIX)/share ;\n"
, "DOCDIR = $(DATADIR)/doc ;\n"
, "HTMLDIR = $(DATADIR)/html ;\n"
, "INCDIR = $(PREFIX)/include ;\n"
, "LIBDIR = $(PREFIX)/lib ;\n"
, "MANDIR = $(DATADIR)/man1 ;\n"
, "\n"
, "######################################################\n"
, "##\n"
, "##  Example definitions for compiler suite (TOOLSETS)\n"
, "##  It is possible to add definitions here or in a\n"
, "##  separate file \"$(TOOLSET).jam\", the NOCARE rule\n"
, "##  tells jam not to worry if file does not exist.\n"
, "## \n"
, "######################################################\n"
, "NOCARE $(TOOLSET).jam ;\n"
, "include $(TOOLSET).jam ;\n"
, "switch $(TOOLSET)\n"
, "{\n"
, "case GNU :\n"
, "CC = gcc ;\n"
, "C++ = g++ ;\n"
, "FORTRAN = gfortran ;\n"
, "LINK = $(C++) ;\n"
, "CCFLAGS += -Wall -std=c99 ;\n"
, "C++FLAGS += -Wall -std=c++98 ;\n"
, "SHRFLAGS = -fpic ;\n"
, "switch $(VARIANT)\n"
, "{\n"
, "case RELEASE :\n"
, "  OPTIM += -O2 -march=native -DDEBUG=0 ;\n"
, "case * :\n"
, "  OPTIM = -O0 -ggdb -DDEBUG=1 ;\n"
, "  CCFLAGS += -pedantic ;\n"
, "  C++FLAGS += ; # -Weffc++ ;\n"
, "}\n"
, "FORTRANFLAGS += -Wall -c ;\n"
, "LINKFLAGS += $(OPTIM) ;\n"
, "OPENMP = -fopenmp ;\n"
, "FORTRANLIBS += -lgfortran ;\n"
, "}\n"
, "\n"
, "##################################################\n"
, "##\n"
, "##  Example of how you might define a set of\n"
, "##  \"standard\" flags for libraries you use often.\n"
, "##\n"
, "##################################################\n"
, "##\n"
, "##switch $(GUILIB)\n"
, "##{\n"
, "##case GTKMM :\n"
, "##  CCFLAGS += `pkg-config gtkmm-2.4 --cflags` ;\n"
, "##  C++FLAGS += `pkg-config gtkmm-2.4 --cflags` ;\n"
, "##  LINKFLAGS += `pkg-config gtkmm-2.4 --libs` ;\n"
, "##\n"
, "##case MOTIF :\n"
, "##  CCFLAGS += -I/usr/include ;\n"
, "##  C++FLAGS += -I/usr/include ;\n"
, "##  LINKFLAGS += -L/usr/lib -lXm -lXp -lXpm -lXmu -lXt -lXext -lX11 ;\n"
, "##\n"
, "##case QT3 :\n"
, "##  CCFLAGS += -I$(QTDIR)/include ;\n"
, "##  C++FLAGS += -I$(QTDIR)/include ;\n"
, "##  LINKFLAGS += -L$(QTDIR)/lib -lqt-mt -lXext -lX11 -lm ;\n"
, "##\n"
, "##case * :\n"
, "##}\n"
, "##\n"
, "\n"
, "# Set variable for variant-specific build-dir handling\n"
, "BASE_LOCATE_TARGET = $(BUILDDIR)$(SLASH)$(VARIANT) ;\n"
, "ALL_LOCATE_TARGET = $(BASE_LOCATE_TARGET) ;\n"
, 0 }
;

/**
 * The name of the jam rules document.
 */
const QString jam_generator::rules_name("Jamrules");

jam_generator::jam_generator()
: lib_defn_ ()
, grist_ ()
, qualified_target_name_ ()
{}

std::unique_ptr< base_generator > jam_generator::create()

{
  std::unique_ptr< base_generator > Result (new jam_generator);
  return Result;
}

void jam_generator::assoc_library(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString & a_includes, QString & a_ldflags, QString & a_cflags) 
{
QString project_;
if( a_target.property_value ( butter_constants::butter_project_name, project_ ) )
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
  find_hdr_link( a_target, a_includes, a_ldflags, a_cflags, jam_generator::style.name(), true );
}
else
{
  {
    QString defn_;
    {
      QTextOStream os_( &defn_ );
      const QString dir_line_
        = this->root_dir().create_relative( a_target.package().src_path() );

      os_ << " <" << pathcmp( dir_line_ ).path_convert( "!" ) << ">"
        << a_target.name () << "$(SUFLIB) \n\t";
    }
    this->lib_defn_.append( defn_ );
  }
  /*  QString libtype_; // Add LOC_TARGET_LDFLAGS variable for static libraries
    if (a_target.property_search (butter_constants::butter_lib_type_name, libtype_)
        && libtype_ != butter_constants::shared_value)*/
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
      QTextOStream os_( &defn_ );
      os_ << " $(" << this->build_target_NAME( a_target.package().filename(), a_target.name() ) << "_LDFLAGS) ";
    }
    a_ldflags.append( defn_ );
  }
  QString libinc_; // Add include variable for both static and shared libraries
  if ( a_target.property_search( butter_constants::butter_include_name, libinc_ ) )
  {
    a_includes.append( libinc_ );
  }
}

}

void jam_generator::assoc_source(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_filename, QString a_basename, QString a_src_inc, QString a_src_flags, bool a_isdoc) 
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
     BUTTER_ALWAYS(a_target.property_value (butter_constants::butter_compiler_name, comp_)
                , ("<p><b>Error:</b> Missing '" + butter_constants::butter_compiler_name
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

void jam_generator::check_properties(bool a_is_source, const ::UmlArtifact & a_source, ::QTextOStream & a_os) 
{

}

void jam_generator::descendent_link(compound_artifact & a_art, compound_artifact & a_sys, const location & a_loc) {
// Need to write the TOP line.
if( NULL != a_loc.parent() )
{
  QString subdir_line_;  // The text of the preamble section (mainly SubDir)
  // Create subpath (eg. src/chemistry) ;
  const QString dir_line2_{ this->root_dir().create_relative( a_loc.full_path() ) };
  // Create a space separated list of folders between here and TOP
  const QString dir_line_{ pathcmp ( dir_line2_ ).path_convert ( " " ) };
  // Reset grist
  this->grist_.truncate( 0 );
  {
    QTextOStream subdir_os_{ &subdir_line_ };
    // Create LOCATE_TARGET
    subdir_os_ << "ALL_LOCATE_TARGET = $(BASE_LOCATE_TARGET)$(SLASH)"
      << pathcmp( dir_line2_ ).path_convert( "$(SLASH)" ) << " ;\n\n"
      << "SubDir TOP " << dir_line_ << " ;\n\n"
      << a_art.document().preamble().value;
  }
  a_art.document().set_preamble_value( subdir_line_ );
  // Add definition to top-level
  a_sys.document().append_close_value( "SubInclude TOP " + dir_line_ + " ;\n" );
}
}

void jam_generator::end_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_include, QString a_ldflags, QString a_cflags, QString a_compiler, base_generator::target_type a_type) 
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
    a_target.property_value (butter_constants::butter_other_name, this->other_target_type_);
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

void jam_generator::initialise(location & a_base, const ::UmlItem & a_project, compound_artifact & a_sys) 
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
  if (a_project.property_value (butter_constants::butter_build_dir_name, prop_value_))
  {
    os_ << "BUILDDIR = " << pathcmp(prop_value_).path_convert ("$(SLASH)") <<" ;\n";
  }
  else
  {
    os_ << "BUILDDIR = " << this->root_dir ().path_convert ("$(SLASH)") <<" ;\n";
  }
  //////////////
  // Set this as top location and bring in Jamrules.
  os_ << "SubDir TOP ;\n";
  os_ << "OUTPUTDIR = $(BUILDDIR)$(SLASH)$(VARIANT) ;\n";
  //////////////
  // Handle user-defined project includes and flag properties
  ////////////
  QString property_value_;
  a_project.property_value (butter_constants::butter_include_name, property_value_);
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
  if (a_project.property_search (butter_constants::butter_ldflags_name, property_value_))
  {
    os_ << "LINKFLAGS += " << property_value_ << " ;\n";
  }
  if (a_project.property_search (butter_constants::butter_flags_name, property_value_))
  {
    os_ << "C++FLAGS += " << property_value_ << " ;\n";
    os_ << "CCFLAGS += " << property_value_ << " ;\n";
  }
}
a_sys.document().set_preamble_value( content_ );

}

void jam_generator::install_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_loc_var, base_generator::install_type a_type, bool a_isdoc) 
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

void jam_generator::start_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_build_file, QString a_compiler, base_generator::target_type a_type) 
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
this->target_NAME( a_target.package().filename(), name_ );

// Target label for non-standard targets
this->other_target_type_.truncate (0);

//////////////////////
// Start entry
a_os << this->target_NAME () << "_SRC =\n\t";

}


} // namespace butter

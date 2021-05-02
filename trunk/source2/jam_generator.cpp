
#include "jam_generator.h"
#include "base_generator.h"
#include "compound_artifact.h"
#include "location.h"

namespace butter {

const basic_style jam_generator::style( "#", "", "##END:", "", "##START:", "", "standard", &butter::jam_generator::create );

const QString jam_generator::build_file_name("Jamfile");

const QString jam_generator::build_file_sysname("Jamfile");

//A template Jamrules.

const char * jam_generator::default_rules[] { "#\n"
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

//The name of the jam rules document.

const QString jam_generator::rules_name("Jamrules");

//Main ctor, takes top-level a_project.
//
//\pre a_project.parent = nul
jam_generator::jam_generator()
: lib_defn_ ()
, grist_ ()
, qualified_target_name_ ()
{
}

//Create bjam generator object.s
std::unique_ptr< base_generator > jam_generator::create()
{
  std::unique_ptr< base_generator > Result (new jam_generator);
  return Result;
}

//** This method a library association to the current target entry for a_target.
//
//Responsibilites
//- Properties
// - associated includes, ldflags, cflags
void jam_generator::assoc_library() {
}

//** This method sets up object for creating a new target entry for a_target.
//
//Responsibilites
//- Properties
// - doc/source: includes, ldflags, cflags
// - compiler
//- Other
// - set compilation for individual 
void jam_generator::assoc_source() {
}

//Check a_source for butter properties and add information to a_os.
//a_is_source is true of a_source stereotype is "source" and false
//if stereotype is "document".
void jam_generator::check_properties() {
}

//Build the inter-buildfile link from this artifact/location to its parent and 
//vice-versa. This is called just before the artifact is closed so has access 
//to the (almost) complete content.
void jam_generator::descendent_link() {
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

//** This method finalises the target entry for a_target.
void jam_generator::end_target() {
}

//In this style external targets are not used, external library 
//data is written in local-targets directly where it is 
//referenced.
//
//Responsibility
//- Properties
// - project (defines external)
// - buildfile
//- Other
// - external target
void jam_generator::external_target() {
}

//Write extra details to the top-level build file.
//
//Responsibilities:
//- Property handling
// - build-dir
// - project: flags, include, ldflags, (library) type
// - style
// - version
void jam_generator::initialise() {
}

//** Create an install target.
//
//Responsibilites
//- Properties
// - install (library, executable and document)
void jam_generator::install_target() {
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
void jam_generator::start_target() {
}


} // namespace butter

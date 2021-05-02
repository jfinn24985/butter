
#include "bjam_generator.h"
#include "base_generator.h"
#include "compound_artifact.h"
#include "location.h"

namespace butter {

const basic_style bjam_generator::style( "#", "", "##END:", "", "##START:", "", "boost", &butter::bjam_generator::create );

//The default leaf filename for the current style

const QString bjam_generator::build_file_name("Jamfile");

//The default leaf filename for the current style

const QString bjam_generator::build_file_sysname("Jamroot");

//The name of the project rules file.

const QString bjam_generator::rules_name("local.jam");

//This is the default contents of a the rules file (local.jam) If a 
//document artifact with name 'local.jam' is not present when 
//butter is executed with \@style='boost' then one will be created
//using this string.

const char * bjam_generator::default_rules[] { "#\n"
, "# local.jam\n"
, "#\n"
, "# Set constants for installation paths\n"
, "import path ;\n"
, "path-constant INSTALL_PREFIX : installdir ;\n"
, "constant BINDIR : bin ; # User programs\n"
, "constant DATADIR : share ; # Private libraries and static data\n"
, "constant DOCDIR : [ path.join share doc ] ;\n"
, "constant HTMLDIR : [ path.join share html ] ;\n"
, "constant INCDIR : include ;\n"
, "constant LIBDIR : lib ; # Shared libraries\n"
, "constant MANDIR  : [ path.join share man1 ] ;\n"
, "\n"
, 0 }
;

//Create bjam generator object.s
std::unique_ptr< base_generator > bjam_generator::create()
{
  std::unique_ptr< base_generator > Result (new bjam_generator);
  return Result;
}

//This method scans a_target for its direct sources and
//direct and indirect dependencies. 
void bjam_generator::assoc_library() {
}

//This method scans a_target for its direct sources and
//direct and indirect dependencies. 
void bjam_generator::assoc_source() {
}

//Write the Jamfile descent links for a_location.
void bjam_generator::descendent_link() {
// Add ourself to the system artifact (if not the top-level).
if ( NULL != a_loc.parent() )
{
  QString tmp_;
  QTextOStream os_( &tmp_ );
  // Add definition for this sub-project to the top-level location
  // use-project /compchem/chemistry : src/chemistry ;

  const QString dir_line_ = root_dir().create_relative( a_loc.full_path () );
  os_ << "use-project /" << project_name_ << "/" << a_loc.path().path() << " : " << dir_line_ << " ;\n";
  os_ << "build-project " << dir_line_ << " ;\n\n";
  a_sys.document().append_close_value( tmp_ );
}
}

//This method scans a_target for its direct sources and
//direct and indirect dependencies. 
void bjam_generator::end_target() {
}

//Write the external a_target at a_location. 
//
//* searches system_artifact_ for an existing target
//* new target sections are add to system_artifact_
void bjam_generator::external_target() {
}

//Write extra details to the top-level build file.

void bjam_generator::initialise() {
}

//Create an install target.
void bjam_generator::install_target() {
}

//This method scans a_target for its direct sources and
//direct and indirect dependencies. 
void bjam_generator::start_target() {
}


} // namespace butter


#include "cmake_generator.h"
#include "compound_artifact.h"
#include "base_generator.h"
#include "location.h"

namespace butter {

const basic_style cmake_generator::style( "#", "", "##END:", "", "##START:", "", "cmake", &butter::cmake_generator::create );

//The default leaf filename for the current style

const QString cmake_generator::build_file_name("CMakeLists.txt");

//The default leaf filename for the current style

const QString & cmake_generator::build_file_sysname(cmake_generator::build_file_name);

//Identify the minimum cmake version this generator supports.

const QString cmake_generator::cmake_minimum_required_("cmake_minimum_required(VERSION 2.6)");

//This is the default contents of a the rules file (M_sys.mak) If a 
//document artifact with name 'M_sys.mak' is not present when 
//butter is executed with \@style='cmake' then one will be created
//using this string.

const char * cmake_generator::default_rules[] { "#\n"
, "# local.cmake\n"
, "#\n"
, "set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_DEBUG DEBUG=1)\n"
, "set_directory_properties(PROPERTIES COMPILE_DEFINITIONS DEBUG=0)\n"
, "#############################\n"
, "##  Default install locations\n"
, "#############################\n"
, "set (CMAKE_INSTALL_PREFIX installdir)\n"
, "set (BINDIR bin)\n"
, "set (DATADIR share)\n"
, "set (DOCDIR share/doc)\n"
, "set (HTMLDIR share/html)\n"
, "set (INCLUDEDIR include)\n"
, "set (LIBDIR lib)\n"
, "set (MANDIR share/man)\n"
, "\n"
, 0 }
;

//The name of the rules file.

const QString cmake_generator::rules_name("local.cmake");

//Create a generator from the top-level a_project
//
//\pre a_project.parent = nul
cmake_generator::cmake_generator()
: lib_set_()
, project_name_()
, depend_set_()
, is_static_type_(false)
, language_set_()
, sys_buildfile_()
{
}

//Create bjam generator object.s
std::unique_ptr< base_generator > cmake_generator::create()
{
  std::unique_ptr< base_generator > Result (new cmake_generator);
  return Result;
}

//** This method a library association to the current target entry for a_target.
//
//Responsibilites
//- Properties
// - associated includes, ldflags, cflags
void cmake_generator::assoc_library() {
}

//** This method sets up object for creating a new target entry for a_target.
//
//Responsibilites
//- Properties
// - doc/source: includes, ldflags, cflags
// - compiler
//- Other
// - set compilation for individual 
void cmake_generator::assoc_source() {
}

//Write the Jamfile descent links for a_location to a_os
void cmake_generator::descendent_link() {
// Keep parent dir definitions.
if ( NULL != a_loc.parent() )
{
  // Add us to base makefile
  QString tmp_;
  QTextOStream link_os_( &tmp_ );
  link_os_ << "add_subdirectory (";
  link_os_ << pathcmp( root_dir().create_relative( a_loc.full_path() ) ).path();
  link_os_ << ")\n";
  a_sys.document().append_close_value( tmp_ );
}


}

//** This method finalises the target entry for a_target.
//
//NOTE: "Other" target requires WORKING_DIRECTORY to allow 
//finding of source files. Alternatively, this could be
//avoided by prepending ${CMAKE_CURRENT_SOURCE_DIR} to each 
//source file. Currently WORKING_DIRECTORY is chosen as
//it was felt that it is more obvious to the user and less
//verbose.
void cmake_generator::end_target() {
}

//In this style external targets are not used, external library 
//data is written in local-targets directly where it is 
//referenced.
void cmake_generator::external_target() {
}

//Write extra details to the top-level build file. 
void cmake_generator::initialise() {
}

//** Create an install target.
//
//Responsibilites
//- Properties
// - install (library, executable and document)
void cmake_generator::install_target() {
}

//Convert input string into CMake allowed string.  This
//means converting () to {} and OUTPUTDIR to 
//CMAKE_BINARY_DIR
QString cmake_generator::mangle() {
const char dollar_('$');
const char escape_('\\');
const char left_paren_('(');
const char right_paren_(')');
std::stack< char > quotes_;
for (unsigned int cursor_(0); cursor_ < input.length (); ++cursor_)
{
  const char c_ = input[cursor_].latin1 ();
  BUTTER_ALWAYS('\0' != c_,"<p><b>Error:</b> No support for non-ASCII/non-latin1 characters.</p>");
  switch (c_)
  {
  case dollar_:
  {
    if (cursor_ + 1 < input.length () && left_paren_ == input[cursor_ + 1])
    {
      input[cursor_ + 1] = '{';
      ++cursor_;
    }
    break;
  }
  case escape_:
  {
    ++cursor_;
    break;
  }
  case '\'':
  case '"':
  case '`':
  {
    if (quotes_.empty () || quotes_.top () != c_)
    {
      quotes_.push (c_);
    }
    else
    {
      quotes_.pop ();
    }
    break;
  }
  case left_paren_:
  {
    quotes_.push (right_paren_);
    break;
  }
  case right_paren_:
  {
    if (quotes_.empty () || quotes_.top () != c_)
    {
      input[cursor_] = '}';
    }
    else
    {
      quotes_.pop ();
    }
    break;
  }
  default: // do nothing
    break;
  }
}

if (! quotes_.empty ())
{
  throw std::runtime_error ("<p><b>Error</b> Unmatched quotes in string: " + std::string(input.utf8()) + "</p>");
}
while (true)
{
  static const QString od_ ("OUTPUTDIR");
  static const QString cd_ ("CMAKE_BINARY_DIR");
  const int found_at_ (input.find (od_));
  if (found_at_ == -1) break;
  input.replace (found_at_, od_.length (), cd_);
}
return input;


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
void cmake_generator::start_target() {
}


} // namespace butter

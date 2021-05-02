
#include "butter/cmake_generator.h"
#include "butter/compound_document.h"
#include "butter/compound_artifact.h"
#include "bouml/UmlArtifact.h"
#include "butter/location.h"
#include "bouml/UmlItem.h"

#include "butter/config.h"
namespace butter {

const basic_style cmake_generator::style( "#", "", "##END:", "", "##START:", "", "cmake", &butter::cmake_generator::create );

/**
 * The default leaf filename for the current style
 */
const QString cmake_generator::build_file_name("CMakeLists.txt");

/**
 * The default leaf filename for the current style
 */
const QString& cmake_generator::build_file_sysname(cmake_generator::build_file_name);

/**
 * Identify the minimum cmake version this generator supports.
 */
const QString cmake_generator::cmake_minimum_required_("cmake_minimum_required(VERSION 2.6)");

/**
 * This is the default contents of a the rules file (M_sys.mak) If a 
 * document artifact with name 'M_sys.mak' is not present when 
 * butter is executed with \@style='cmake' then one will be created
 * using this string.
 */
const char * cmake_generator::default_rules[]{ "#\n"
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

/**
 * The name of the rules file.
 */
const QString cmake_generator::rules_name("local.cmake");

cmake_generator::cmake_generator()
: lib_set_()
, project_name_()
, depend_set_()
, is_static_type_(false)
, language_set_()
, sys_buildfile_()
{}

std::unique_ptr< base_generator > cmake_generator::create()

{
  std::unique_ptr< base_generator > Result (new cmake_generator);
  return Result;
}

void cmake_generator::assoc_library(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString & a_includes, QString & a_ldflags, QString & a_cflags) {
////////////////////////////////////
// Define the associations of target
//
// to sources and libraries
//
QString project_;
if (a_target.property_value (butter_constants::butter_project_name, project_))
{
  //////////////////////////////////
  // Is an externally linked library
  //
  // add flags to main target.
  find_hdr_link (a_target, a_includes, a_ldflags, a_cflags, cmake_generator::style.name(), true);

  QString name_ (a_target.name ());
  if (! lib_set_.isEmpty ())
  {
    lib_set_.append (" ");
  }
  lib_set_.append("${" + project_.upper() + "_LINK_LIBS}");
}
else
{
  QString name_ (a_target.name ());
  ///////////////////
  // Is local library target, add as a dependency
  if (!depend_set_.isEmpty ())
  {
    depend_set_.append (" ");
  }
  depend_set_.append(name_);
  //
  // Create linker information
  if (!lib_set_.isEmpty ())
  {
    lib_set_.append (" ");
  }
  lib_set_.append(name_);

  // Get any flags that we might need
  QString library_type_;
  if (a_target.property_search (butter_constants::butter_lib_type_name, library_type_)
      && butter_constants::static_value == library_type_)
  {
    //////////////////////
    // Is a static library so we need to get
    // ldflags from static library's sources
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
  }
  // Both static/shared add include and link flags, ignore cflags
  QString src_flags_;
  find_hdr_link (a_target, a_includes, a_ldflags, src_flags_, cmake_generator::style.name(), true);
}
}

void cmake_generator::assoc_source(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_filename, QString a_basename, QString a_src_inc, QString a_src_flags, bool a_isdoc) {
////////////////////////////////////
// Define the associations of target
//
// to sources and documents
//

/*
  if (a_isdoc)
  {
    /////////////////////////////////
    // Add document to TARGET_OBJ as they may
    // not be deduced from source list in case
    // they are not deduced for "clean" target
    //
    // Assume object just replaces extension
    user_obj_ << this->target_NAME () << "_OBJ := $(" << this->target_NAME () << "_OBJ)  " << a_basename << "$(sufobj)\n\n";
  }
*/

// Add source/document
a_os << "\n\t" << a_filename;

///////////////////////////////
// If src has hdr defined
// add to target's includes.
if( ! a_src_inc.isEmpty() )
{
  QString user_extra_;
  {
    QTextOStream user_obj_( &user_extra_ );
    user_obj_ << "\ninclude_directories (";
    for( const_token_iterator e_, b_( a_src_inc, ' ' ); e_ != b_; ++b_ )
    {
      const QString val_( b_->c_str () );
      if( ! val_.isEmpty() )
      {
        if( QChar( '$' ) != val_[0] && QDir::isRelativePath( val_ ) )
        {
          user_obj_ << "${CMAKE_SOURCE_DIR}/";
        }
        user_obj_ << this->mangle( val_ ) << " ";
      }
    }
    user_obj_ << ")\n";
  }
  this->individual_obj_.append( user_extra_ );
}

///////////////////////////////
// If src has flags or is a doc with compiler set means we
// have to add properties for it.
// set_source_files_properties (name COMPILE_FLAGS .. COMPILE_DEFINITIONS ..)
QString comp_;
if( a_isdoc )
{
  // Check if user has set the language,
  // if not assume it will be autodetected
  a_target.property_value( butter_constants::butter_compiler_name, comp_ );
}

if( ! a_src_flags.isEmpty() || ! comp_.isEmpty() )
{
  QString user_extra_;
  {
    QTextOStream user_obj_( &user_extra_ );
    user_obj_ << "set_source_files_properties (" << a_filename
      << "\n\tPROPERTIES";
    if( ! comp_.isEmpty() && "CCC" != comp_ )
    {
      if( "CC" == comp_ )
      {
        comp_ = "C";
      }
      else
      {
        comp_ = comp_.lower();
        comp_[0] = comp_[0].upper ();
      }
      if( 0 == this->language_set_.contains( comp_ ) )
      {
        this->language_set_.append( comp_ );
        // Update project header
        QString init_text_;
        {
          QTextOStream init_os_( &init_text_ );
          init_os_ << this->cmake_minimum_required_ << "\n\n"
            << "project (" << this->project_name_;
          // Handle languages if necessary
          if( ! this->language_set_.isEmpty () )
          {
            init_os_ << " CXX " << this->language_set_.join( " " );
          }
          init_os_ << ")\n\n";
        }
        this->sys_buildfile_->document().set_preamble_value( init_text_ );
      }
      user_obj_ << "\n\tLANGUAGE " << comp_;
    }
    if ( ! a_src_flags.isEmpty () )
    {
      user_obj_ << "\n\tCOMPILE_FLAGS " << mangle ( a_src_flags );
    }
    user_obj_ << ")\n\n";
  }
  this->individual_obj_.append ( user_extra_ );
}
}

void cmake_generator::descendent_link(compound_artifact & a_art, compound_artifact & a_sys, const location & a_loc) 
{
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

void cmake_generator::end_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_include, QString a_ldflags, QString a_cflags, QString a_compiler, base_generator::target_type a_type) {
////////////////////////////////
// Finalise target build instructions
//
a_os << ")\n";
if (a_type == other)
{
  ////////////////
  // Handle "other" library types specially.
  QString other_type_ (this->other_target_type_.upper ());
  other_type_.replace( QChar('-'), QChar('_') );
  a_os << "add_custom_command (OUTPUT ${" << this->target_NAME ()
      << "}\n\t DEPENDS ${" << this->target_NAME () << "_SRC} "
      << this->lib_set_
      << "\n\tCOMMAND ${" << other_type_ << "} ";
  if (! a_include.isEmpty ())
  {
    a_os << mangle(a_include) << " ";
  }
  if (! a_cflags.isEmpty ())
  {
    a_os << mangle(a_cflags) << " ";
  }
  if (! a_ldflags.isEmpty ())
  {
    a_os << mangle(a_ldflags) << " ";
  }
  a_os << "${" << other_type_ << "FLAGS} ${CMAKE_CURRENT_BINARY_DIR}/${" << this->target_NAME ()
      << "} ${" << this->target_NAME () << "_SRC}\t\n"
      << "WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})\n"
      << "set_directory_properties (PROPERTIES\n\tADDITIONAL_MAKE_CLEAN_FILES ${"
      << this->target_NAME () << "})\n"
      << "add_custom_target (${" << this->target_NAME ()
      << "}_TARGET ALL\n\tDEPENDS ${" << this->target_NAME ()
      << "})\n";
}
else
{
  switch (a_type)
  {
  case executable:
    {
      a_os << "add_executable (${" << this->target_NAME () << "}"
              << "\n\t${" << this->target_NAME () << "_SRC})\n";
    }
    break;
  case static_library:
    {
      a_os << "add_library (${" << this->target_NAME () << "}"
              << "\n\tSTATIC ${" << this->target_NAME () << "_SRC})\n";
      a_os << "set_target_properties(${" << target_NAME () << "} PROPERTIES PREFIX \"\")\n";
    }
    break;
  case shared_library:
    {
      a_os << "add_library (${" << this->target_NAME () << "}"
              << "\n\tSHARED ${" << this->target_NAME () << "_SRC})\n";
      a_os << "set_target_properties(${" << target_NAME () << "} PROPERTIES PREFIX \"\")\n";
    }
    break;
  case other: // Can not get here.
    break;
  }
  // Create linker and compiler flagsets
  if (! this->lib_set_.isEmpty () || ! a_ldflags.isEmpty ())
  {
    a_os << "target_link_libraries (${" << this->target_NAME () << "}";
    if (! this->lib_set_.isEmpty ())
    {
      a_os << "\n\t" << mangle(this->lib_set_);
    }
    if (! a_ldflags.isEmpty ())
    {
      a_os << "\n\t" << mangle(a_ldflags);
    }
    a_os << ")\n";
  }
  if (! a_cflags.isEmpty ())
  {
    a_os << "set_target_properties(${" << target_NAME () << "}"
      << "\n\tPROPERTIES"
      << "\n\tCOMPILE_FLAGS " << mangle(a_cflags) << ")\n";
  }
  if (! a_include.isEmpty ())
  {
    a_os << "\ninclude_directories (";
    for (const_token_iterator e_, b_(a_include, ' '); e_ != b_; ++b_)
    {
      const QString val_(b_->c_str ());
      if (!val_.isEmpty ())
      {
        if (QChar('$') != val_[0] && QDir::isRelativePath (val_))
        {
          a_os << "${CMAKE_SOURCE_DIR}/";
        }
        a_os << mangle (val_) << " ";
      }
    }
    a_os << ")\n";
  }

  // Add any object definitions.
  if (! this->individual_obj_.isEmpty ())
  {
    a_os << this->individual_obj_ << "\n";
  }
}

}

void cmake_generator::initialise(location & a_base, const ::UmlItem & a_project, compound_artifact & a_sys) 
{
BUTTER_REQUIRE( NULL == a_base.parent(), "initialise can only be called on the top-most location" );
this->project_name_ = a_project.name().upper();
this->sys_buildfile_ = &a_sys;
if( a_sys.document().preamble().value.isEmpty() )
{
  QString init_text_;
  {
    QTextOStream init_os_( &init_text_ );
    init_os_ << this->cmake_minimum_required_ << "\n\n"
      << "project (" << this->project_name_ << ")\n\n";
  }
  a_sys.document().set_preamble_value( init_text_ );
}
QString proj_text_;
{
  QTextOStream proj_os_( &proj_text_ );
  proj_os_ << "include (" << ( pathcmp( "${CMAKE_SOURCE_DIR}" )
      / this->rules_name ).path() << ")\n\n";

  const pathcmp base_include_( dynamic_cast< const UmlPackage & >( a_project ).hdr_path() );
  {
    QString hdrs_;
    a_project.property_value( butter_constants::butter_include_name, hdrs_ );
    if( ! this->root_dir().equality( base_include_ ) )
    {
      if( ! hdrs_.isEmpty() )
      {
        hdrs_.append( " " );
      }
      hdrs_.append( this->root_dir().create_relative( base_include_ ) );
    }
    if ( ! hdrs_.isEmpty() )
    {
      proj_os_ << "\ninclude_directories (";
      for( const_token_iterator e_, b_( hdrs_, ' ' ); e_ != b_; ++b_ )
      {
        const QString val_( b_->c_str() );
        if ( ! val_.isEmpty() )
        {
          if( QChar( '$' ) != val_[0] && QDir::isRelativePath( val_ ) )
          {
            proj_os_ << "${CMAKE_SOURCE_DIR}/";
          }
          proj_os_ << mangle( val_ ) << " ";
        }
      }
      proj_os_ << ")\n";
    }
  }
  {
    QString link_;
    if( a_project.property_search( butter_constants::butter_ldflags_name, link_ ) )
    {
      proj_os_ << "set (CMAKE_EXE_LINKER_FLAGS " << mangle( link_ )
        << "\n\tCACHE STRING \"Project-wide linker flags.\" FORCE)\n";
    }
  }
  {
    QString flag_;
    if( a_project.property_search( butter_constants::butter_flags_name, flag_ ) )
    {
      proj_os_ << "add_definitions (" << mangle( flag_ ) << ")\n";
    }
  }
}
if( a_sys.document().has_target( this->project_name_ ) )
{
  a_sys.document().set_target_value( this->project_name_, proj_text_ );
}
else
{
  a_sys.document().add_target( this->project_name_, {}, proj_text_ );
}

}

void cmake_generator::install_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_loc_var, base_generator::install_type a_type, bool a_isdoc) 
{
const QString target_name_(a_isdoc ?  a_target.name() : "${" + this->target_NAME () + "}");
switch (a_type)
{
case base_generator::bin:
{
  a_os << "install (TARGETS " << target_name_
  << "\n\tRUNTIME DESTINATION ${" << a_loc_var << "}"
  << "\n\tCONFIGURATIONS Release)\n\n";
  break;
}
case base_generator::lib:
{
  QString shard_, statd_;
  if (this->is_static_type_)
  {
    statd_ = a_loc_var;
    shard_ = "LIBDIR";
  }
  else
  {
    statd_ = "DATADIR";
    shard_ = a_loc_var;
  }
  a_os << "install (TARGETS " << target_name_
  << "\n\tLIBRARY DESTINATION ${" << shard_ << "}"
  << "\n\tCONFIGURATIONS Release"
  << "\n\tARCHIVE DESTINATION ${" << statd_ << "}"
  << "\n\tCONFIGURATIONS Release)\n\n";
  break;
}
case base_generator::file:
case base_generator::man:
{
  a_os << "install (FILES " << target_name_
  << "\n\tDESTINATION ${" << a_loc_var << "}"
  << "\n\tCONFIGURATIONS Release)\n\n";
  break;
}
}

}

QString cmake_generator::mangle(QString input) 
{
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

void cmake_generator::start_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_build_file, QString a_compiler, base_generator::target_type a_type) {
/////////////////
// Initialise variables for the new target
this->target_NAME( a_target.package().filename(), a_target.name() );
this->other_target_type_.truncate (0);    // An "other" target type.
this->lib_set_.truncate (0);              // Set of included libraries
this->individual_obj_.truncate (0);       // Set of sources that need to be individually compiled
this->depend_set_.truncate (0);        // local targets
this->is_static_type_ = (a_type == base_generator::static_library);
///////////////////////////////////////////
// Define the characteristics of the target
//
// a_target must be executable or library stereotype!
//
if (a_type == other)
{
  a_target.property_value (butter_constants::butter_other_name, this->other_target_type_);
}

// Handle target specific include file for library types only
if (a_type != executable)
{
  if (! a_build_file.isEmpty ())
  {
    a_os << "include (" << (pathcmp("${CMAKE_SOURCE_DIR}") / a_build_file).path () << ".cmake)\n\n";
  }
}

//////////////////
// Start the source file associations
a_os << "set (" << this->target_NAME () << " " << a_target.name () << ")\n\n";
a_os << "set (" << this->target_NAME () << "_SRC ";
}


} // namespace butter

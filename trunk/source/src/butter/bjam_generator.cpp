/**
 * Boost Jam generator source file.
 */
#include "butter/bjam_generator.h"
#include "bouml/UmlArtifact.h"
#include "butter/compound_artifact.h"
#include "butter/location.h"
#include "bouml/UmlItem.h"

// Manual source includes
#include "butter/config.h"
// -
namespace butter {

const basic_style bjam_generator::style( "#", "", "##END:", "", "##START:", "", "boost", &butter::bjam_generator::create );

/**
 * The default leaf filename for the current style
 */
const QString bjam_generator::build_file_name("Jamfile");

/**
 * The default leaf filename for the current style
 */
const QString bjam_generator::build_file_sysname("Jamroot");

/**
 * The name of the project rules file.
 */
const QString bjam_generator::rules_name("local.jam");

/**
 * This is the default contents of a the rules file (local.jam) If a 
 * document artifact with name 'local.jam' is not present when 
 * butter is executed with \@style='boost' then one will be created
 * using this string.
 */
const char * bjam_generator::default_rules[] = { "#\n"
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
, "constant LIBDIR : bin ; # Shared libraries\n"
, "constant MANDIR  : [ path.join share man1 ] ;\n"
, "\n"
, 0 }
;

std::unique_ptr< base_generator > bjam_generator::create()

{
  std::unique_ptr< base_generator > Result (new bjam_generator);
  return Result;
}

void bjam_generator::assoc_library(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString & a_includes, QString & a_ldflags, QString & a_cflags) {
QString project_;
if (a_target.property_value (butter_constants::butter_project_name, project_))
{
  a_os << "/" << project_ << "//";
}
else
{
  a_os << "/" << this->project_name_ << "/";
  const QString loc_ (a_target.package ().filename ());
  if (loc_ == ".")
    a_os << "/";
  else
    a_os << loc_ << "//";
}
a_os << a_target.name () << "\n\t";
/////////////
// Note. Header/link information is imported with library in boost jam.
}

void bjam_generator::assoc_source(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_filename, QString a_basename, QString a_src_inc, QString a_src_flags, bool a_isdoc) {
QString individual_;
if (! this->is_other_ && (! a_src_inc.isEmpty () && ! a_src_flags.isEmpty ()))
{
  // Add individual obj description
  QString compiler_ ("cxx");
  QTextOStream source_os_ (&individual_);
  BUTTER_ALWAYS(a_target.property_value (butter_constants::butter_compiler_name, compiler_) || ! a_isdoc
                , ("<p><b>Error:</b> Missing '" + butter_constants::butter_compiler_name
                                + "' property on " + QString (a_filename)).utf8 ().data ());
  if (compiler_ == "CC") compiler_ = "C";
  if (compiler_ == "FORTRAN") compiler_ = "F";
  source_os_ << "obj " << a_basename << "_obj : "
              << a_filename << " : ";
  if (! a_src_inc.isEmpty ())
  {
    for (const_token_iterator e_, b_(a_src_inc, ' '); b_ != e_; ++b_)
    {
      const QString path_(b_->c_str ());
      if (! path_.isEmpty ())
      {
        source_os_ << "<include>";
        if (QDir::isRelativePath (path_)) source_os_ << "$(topdir)/";
        source_os_ << "\"" << path_ << "\" ";
      }
    }
  }
  source_os_ << "$(" << this->target_NAME () << "HDRS) ";
  if (! a_src_flags.isEmpty ())
  {
    source_os_ << "<" << compiler_.lower () << "flags>\"" << a_src_flags
               << "\" $(" << this->target_NAME () << "FLAGS) ";
  }
  source_os_ << ";\n";
  // Add obj to target
  a_os << a_basename << "_obj\n\t";
}
else
{
  a_os << a_filename << "\n\t";
}

if (! individual_.isEmpty ())
{
  this->individual_obj_.append (individual_);
}
}

void bjam_generator::descendent_link(compound_artifact & a_art, compound_artifact & a_sys, const location & a_loc) {
// Add ourself to the system artifact (if not the top-level).
if (NULL != a_loc.parent ())
{
  QString tmp_;
  QTextOStream os_ (&tmp_);
  // Add definition for this sub-project to the top-level location
  // use-project /compchem/chemistry : src/chemistry ;

  const QString dir_line_ = root_dir().create_relative (a_loc.full_path ());
  os_ << "use-project /" << project_name_ << "/" << a_loc.path ().path () << " : " << dir_line_ << " ;\n";
  os_ << "build-project " << dir_line_ << " ;\n\n";
  a_sys.close.second.append (tmp_);
}
}

void bjam_generator::end_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_include, QString a_ldflags, QString a_cflags, QString a_compiler, base_generator::target_type a_type) {
///////////////////////
// end target definition
a_os << ":\n\t"; // End sources.
// Standard requirements
if (! a_include.isEmpty ())
{
  for (const_token_iterator e_, b_(a_include, ' '); b_ != e_; ++b_)
  {
    const QString flag_(b_->c_str ());
    if (! flag_.isEmpty ())
    {
      a_os << "<include>";
      if (QDir::isRelativePath (flag_)) a_os << "$(topdir)/";
      a_os << "\"" << flag_ << "\" ";
    }
  }
  a_os << "\n\t";
}
if (! a_cflags.isEmpty ())
{
  if (other == a_type)
  {
    // Handle flags for non-standard targets.
    const QString name_ ("<" + this->other_target_type_ + "flags>\"");
    for (const_token_iterator e_, b_(a_cflags, ' '); b_ != e_; ++b_)
    {
      const QString flag_(b_->c_str ());
      if (! flag_.isEmpty ())
      {
        a_os << name_ << flag_ << "\" ";
      }
    }
    a_os << "\n\t";
  }
  else
  {
    for (const_token_iterator e_, b_(a_cflags, ' '); b_ != e_; ++b_)
    {
      const QString flag_(b_->c_str ());
      if (! flag_.isEmpty ())
      {
        a_os << "<cxxflags>\"" << flag_ << "\" " << "<cflags>\"" << flag_ << "\"\n\t";
      }
    }
  }
}
// Handle share/static libraries
switch (a_type)
{
case static_library:
  {
    a_os << "<link>static\n\t: :\n\t";
    if (! a_ldflags.isEmpty ())
    {
      // Put linker and include flags as pass-to-user
      for (const_token_iterator e_, b_(a_ldflags, ' '); b_ != e_; ++b_)
      {
        const QString flag_(b_->c_str ());
        if (! flag_.isEmpty ())
        {
          a_os << "<linkflags>\"" << flag_ << "\" ";
        }
      }
      a_os << "\n\t";
    }
    QString headers_;
    if (a_target.property_value (butter_constants::butter_include_name, headers_))
    {
      for (const_token_iterator e_, b_(headers_, ' '); b_ != e_; ++b_)
      {
        const QString flag_(b_->c_str ());
        if (! flag_.isEmpty ())
        {
          a_os << "<include>";
          if (QDir::isRelativePath (flag_)) a_os << "$(topdir)/";
          a_os << "\"" << flag_ << "\" ";
        }
      }
      a_os << "\n\t";
    }
  }
  break;

case shared_library:
  {
    a_os << "<link>shared\n\t";
    if (! a_ldflags.isEmpty ())
    {
      // Use linker flags but pass on any headers.
      for (const_token_iterator e_, b_(a_ldflags, ' '); b_ != e_; ++b_)
      {
        const QString flag_(b_->c_str ());
        if (! flag_.isEmpty ())
        {
          a_os << "<linkflags>\"" << flag_ << "\" ";
        }
      }
      a_os << "\n\t";
    }
    // Pass only the includes defined on the library to the library user
    QString headers_;
    if (a_target.property_value (butter_constants::butter_include_name, headers_))
    {
      a_os << ": :";
      for (const_token_iterator e_, b_(headers_, ' '); b_ != e_; ++b_)
      {
        const QString flag_(b_->c_str ());
        if (! flag_.isEmpty ())
        {
          a_os << "<include>";
          if (QDir::isRelativePath (flag_)) a_os << "$(topdir)/";
          a_os << "\"" << flag_ << "\" ";
        }
      }
      a_os << "\n\t";
    }
  }
  break;
case executable:
case other:
  if (! a_ldflags.isEmpty ())
  {
    for (const_token_iterator e_, b_(a_ldflags, ' '); b_ != e_; ++b_)
    {
      const QString flag_(b_->c_str ());
      if (! flag_.isEmpty ())
      {
        a_os << "<linkflags>\"" << flag_ << "\" ";
      }
    }
    a_os << "\n\t";
  }
  break;
}
a_os << ";\n\n";
// Add any object definitions
if (! this->individual_obj_.isEmpty ())
{
  // Add variables for flags to use with obj
  a_os << "local " << this->target_NAME () << "HDRS = ";
  QString headers_;
  if (a_target.property_value (butter_constants::butter_include_name, headers_))
  {
    for (const_token_iterator e_, b_(headers_, ' '); b_ != e_; ++b_)
    {
      const QString flag_(b_->c_str ());
      if (! flag_.isEmpty ())
      {
        a_os << "<include>";
        if (QDir::isRelativePath (flag_)) a_os << "$(topdir)/";
        a_os << "\"" << flag_ << "\" \n\t";
      }
    }
  }
  a_os << ";\n";
  a_os << "local " << this->target_NAME () << "FLAGS = ";
  QString flags_;
  if (a_target.property_value (butter_constants::butter_flags_name, flags_))
  {
    for (const_token_iterator e_, b_(flags_, ' '); b_ != e_; ++b_)
    {
      const QString flag_(b_->c_str ());
      if (! flag_.isEmpty ())
      {
        a_os << "<cxxflags>\"" << flag_ << "\" " << "<cflags>\"" << flag_ << "\"\n\t";
      }
    }
  }
  a_os << " ;\n\n";
  a_os << this->individual_obj_;
}
}

void bjam_generator::external_target(const location & a_current, const ::UmlArtifact & a_target, compound_artifact & a_sys) {
// Get project name, if present
QString value_;
a_target.property_value( butter_constants::butter_project_name, value_ );
BUTTER_CHECK( ! value_.isEmpty (), "Programming error, should not be in external_target if project attribute is not set on artifact." );
// Get the external target section
butter::compound_artifact::string_pair_t & proj_ = a_sys.target( value_ );
// Construct a mini-project section.
if( proj_.second.isEmpty () )
{
  proj_.second.append( "#\n# Section for project: " + value_ + "\n#\nproject " + value_ + " ;\n" );
}
// If this is a using directive - we don't need to do anything
if( ! proj_.second.contains ( "using " ) )
{
  QString tmp_;
  if( a_target.property_value( butter_constants::butter_buildfile_name, tmp_ ) )
  {
    // This is a using directive; all definitions are assumed to be in the
    // given jamfile -> reset string
    proj_.second = QString ( "#\n# Section for project: " + value_ + "\n#\nproject " + value_ + " ;\nusing " + tmp_ + " ;\n" );
  }
  else
  {
    QString template_( section( this->style.name(), const_cast< UmlArtifact & >( a_target ).description() ) );
    if( ! template_.isEmpty() )
    {
      proj_.second.append( template_ + "\n" );
    }
    else
    {
      QString includes_, linkflags_, cflags_;
      find_hdr_link( a_target, includes_, linkflags_, cflags_, this->style.name(), true );
      if( ! ( includes_.isEmpty() && linkflags_.isEmpty() ) )
      {
        proj_.second.append( "alias " + a_target.name() + " : : : : " );
        if( ! includes_.isEmpty () )
        {
          proj_.second.append( "<include>\"" + includes_ + "\" " );
        }
        if( ! linkflags_.isEmpty() )
        {
          proj_.second.append( "<linkflags>\"" + linkflags_ + "\" " );
        }
        if( ! cflags_.isEmpty() )
        {
          proj_.second.append( "<cxxflags>\"" + cflags_ + "\" <cflags>\"" + cflags_ + "\" " );
        }
        proj_.second.append( ";\n\n" );
      }
    }
  }
}
}

void bjam_generator::initialise(location & a_base, const ::UmlItem & a_project, compound_artifact & a_sys) {
BUTTER_REQUIRE( NULL == a_base.parent(), "initialise can only be called on the top-most location" );
this->project_name_ = a_project.name();
QString content_;
{
  QString base_include_( a_base.full_path().create_relative( dynamic_cast< const UmlPackage & >( a_project ).hdr_path() ) );
  QTextOStream os_( &content_ );
  {
    const int e_ = rules_name.find( ' ' );
    QString name_( e_ < 0 ? rules_name : rules_name.left( e_ ) );
    os_ << "# Include local constants\npath-constant topdir : . ;\ninclude " << name_ << " ;\n\n";
  }
  os_ << "# Set project's global settings\nproject " << project_name_
    << "\n\t: requirements <debug-symbols>on:<define>\"DEBUG=1\""
    << "\n\t<debug-symbols>off:<define>\"DEBUG=0\"\n\t";
  QString link_, flag_; // Needed arguments.
  find_hdr_link( a_project, base_include_, link_, flag_, this->style.name(), false );
  if ( ! base_include_.isEmpty() )
  {
    for( const_token_iterator e_, b_( base_include_, ' ' ); b_ != e_; ++b_ )
    {
      const QString path_( b_->c_str () );
      if ( ! path_.isEmpty() )
      {
        os_ << "<include>";
        if ( QDir::isRelativePath( path_ ) )
        {
          os_ << "$(topdir)/";
        }
        os_ << "\"" << path_ << "\" ";
      }
    }
    os_ << "\n\t";
  }
  if ( ! link_.isEmpty() )
  {
    for ( const_token_iterator e_, b_( link_, ' ' ); b_ != e_; ++b_ )
    {
      const QString path_( b_->c_str () );
      if ( ! path_.isEmpty() )
      {
        os_ << "<linkflags>\"" << path_ << "\" ";
      }
    }
    os_ << "\n\t";
  }
  if ( ! flag_.isEmpty () )
  {
    for ( const_token_iterator e_, b_( flag_, ' ' ); b_ != e_; ++b_ )
    {
      const QString path_( b_->c_str () );
      if ( ! path_.isEmpty() )
      {
        os_ << "<cxxflags>\"" << path_ << "\" <cflags>\"" << path_ << "\"\n\t";
      }
    }
  }
  ///////////
  // Handle build directory with default to "build"
  QString prop_value_;
  if ( a_project.property_value( butter_constants::butter_build_dir_name, prop_value_ ) )
  {
    os_ << ": build-dir " << prop_value_ << " ;\n";
  }
  else
  {
    // Always put in a default build directory.
    os_ << ": build-dir build ;\n";
  }
}
a_sys.preamble.second = content_;
}

void bjam_generator::install_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_loc_var, base_generator::install_type a_type, bool a_isdoc) 
{
const QString name_ (a_target.name ());
a_os << "install install_" << name_ << " : " << name_
        << " : <variant>release:<location>$(INSTALL_PREFIX)/$(" << a_loc_var << ") \n\t\t"
      << "<variant>debug:<location>$(DEBUG_PREFIX)/\""
      << root_dir ().create_relative (a_target.package ().src_path ()) << "\" ;\n";

}

void bjam_generator::start_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_build_file, QString a_compiler, base_generator::target_type a_type) {
////////////////////////
// Reset variables
this->other_target_type_.truncate (0);
this->individual_obj_.truncate (0);
this->target_NAME (a_target);
this->is_other_ = (a_type == other);
///////////////////////
// Start target definition
const QString stereotype_ (const_cast< UmlArtifact& >(a_target).stereotype ());
if (stereotype_ == butter_constants::library_stereotype)
{
  if (a_target.property_value (butter_constants::butter_other_name, this->other_target_type_))
  {
    if (! a_build_file.isEmpty ())
    {
      a_os << "include " << a_build_file << ".bjam ;\n\n";
    }
    a_os << this->other_target_type_ << " ";
  }
  else
  {
    a_os << "lib ";
  }
}
else if (stereotype_ == butter_constants::executable_stereotype)
{
  a_os << "exe ";
}
a_os << a_target.name () << " :\n\t";
}


} // namespace butter

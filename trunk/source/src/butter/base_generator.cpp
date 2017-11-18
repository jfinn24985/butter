/**
 * Base generator source file
 */
#include "butter/base_generator.h"
#include "butter/butter_constants.h"
#include "butter/compound_document.h"
#include "butter/const_token_iterator.h"
#include "butter/utility.h"
#include "butter/location.h"
#include "bouml/UmlItem.h"
#include "butter/compound_artifact.h"
#include "bouml/UmlArtifact.h"

// Manual includes
#include <qregexp.h>
#include <stdexcept>
#include <qstack.h>
#include "butter/config.h"
// --
namespace butter {

void base_generator::find_hdr_link(const ::UmlItem & a_item, QString & a_hdr, QString & a_link, QString & a_flag, QString a_section_label, bool a_not_doc)
{
// Process any data from properties.
QString value_;
if (a_item.property_value (butter_constants::butter_flags_name, value_))
{
  merge_string_list (a_flag, value_);
}
if (a_item.property_value (butter_constants::butter_ldflags_name, value_))
{
  merge_string_list (a_link, value_);
}
if (a_item.property_value (butter_constants::butter_include_name, value_))
{
  merge_string_list (a_hdr, value_);
}
// For non-documents any data from the description.
if (a_not_doc)
{
  QString section_ (section (a_section_label, const_cast< UmlItem& >(a_item).description ()));
  if (section_.isEmpty ())
  {
    section_ = section (butter_constants::generic_section, const_cast< UmlItem& >(a_item).description ());
  }
  if (! section_.isEmpty ())
  {
    QRegExp hdr_re_ ("^ *" + butter_constants::header_label + " *= *");
    QRegExp link_re_ ("^ *" + butter_constants::link_label + " *= *");
    QRegExp flag_re_ ("^ *" + butter_constants::flag_label + " *= *");
    if (! section_.isEmpty ())
    {
      QTextIStream section_is_ (&section_);
      while (! section_is_.atEnd ())
      {
        const QString line_ (section_is_.readLine ());
        int length_;
        if (-1 != hdr_re_.match (line_, 0, &length_))
        {
          merge_string_list (a_hdr, line_.mid (length_));
        }
        else if (-1 != link_re_.match (line_, 0, &length_))
        {
          merge_string_list (a_link, line_.mid (length_));
        }
        else if (-1 != flag_re_.match (line_, 0, &length_))
        {
          merge_string_list (a_flag, line_.mid (length_));
        }
      }
    }
  }
}
}

std::unique_ptr< compound_artifact > base_generator::get_artifact(location & a_loc, QString a_name)

{
// Get deployment views from current location's packages
std::unique_ptr< compound_artifact > Result;
if( a_loc.packages().count() > 0 )
{
  QVector< UmlItem > docs_ = a_loc.find_uml_document( a_name );
  // If any documents found
  if( ! docs_.isEmpty() )
  {
    Result = std::unique_ptr< compound_artifact >( new compound_artifact( dynamic_cast< UmlArtifact & >( *docs_.at( 0 ) ) ) );
    for( unsigned int k_ = 1; k_ < docs_.count(); ++k_ )
    {
      UmlItem * item_ = docs_.at( k_ );
      Result->merge( dynamic_cast< UmlArtifact & >( *item_ ) );
      log::com.trace( log::info, "<p><b>Note:</b> Removing duplicated build file from Package " + item_->parent()->name() + "</p>\n" );
      // item_->parent()->children().remove(item_->parent ()->children().findRef(item_));
      item_->set_Name( ( "OLD_" + a_name ).utf8() );
    }
    // Reset the artifact (keeps section labels but empties content)
    Result->document().reset();
  }
}
if( NULL == Result.get() )
{
  // This will throw an error if no packages exist.
  // Result.reset (new compound_artifact (*a_loc.create_uml_document (a_name)));
  Result = std::unique_ptr< compound_artifact >( new compound_artifact( *a_loc.create_uml_document( a_name ) ) );
}
return Result;

}

void base_generator::merge_string_list(QString & a_list, QString a_addition)
{
if (a_list.isEmpty ())
{
  a_list = a_addition.simplifyWhiteSpace ();
}
else
{
  for (const_token_iterator e1_, b1_(a_addition.simplifyWhiteSpace (), ' '); b1_ != e1_; ++b1_)
  {
    const QString new_item_(b1_->c_str ());
    if (! new_item_.isEmpty ())
    {
      bool match_ (false);
      for (const_token_iterator e2_, b2_(a_list, ' '); b2_ != e2_; ++b2_)
      {
        const QString orig_(b2_->c_str ());
        if (! orig_.isEmpty ())
        {
          if (orig_ == new_item_)
          {
            match_ = true;
            break;
          }
        }
      }
      if (!match_)
      {
        a_list.append (" ");
        a_list.append (new_item_);
      }
    }
  }
}
}

QString base_generator::section(QString a_section, QString a_desc)
{
static const QString marker_start_ ("${" + butter_constants::section_prefix);
const QString marker_phrase_ (marker_start_ + a_section + "}");
QString Result;
int l_start (a_desc.find (marker_phrase_));
if (-1 != l_start)
{
  // Found a_section start, search from here to end for next section
  int l_end (a_desc.find (marker_start_, l_start + marker_phrase_.length ()));
  if (-1 != l_end)
  {
    Result = a_desc.mid (l_start + marker_phrase_.length (), l_end - (l_start + marker_phrase_.length ()));
  }
  else
  {
    Result = a_desc.mid (l_start + marker_phrase_.length ());
  }
}
// else ...
// Found no section titles --> return empty string
return Result;

}

void base_generator::root_dir(const pathcmp & a_path) 
{
  BUTTER_ENSURE (! a_path.path ().isEmpty ()
  , "<p><b>Program error:</b> Attempt to set project root directory to an empty path.</p>");
  root_dir_ = std::unique_ptr < pathcmp >(new pathcmp (a_path));
}

QString base_generator::to_target_NAME(const ::UmlArtifact & a_target)
{
  QString Result (a_target.package ().filename ());
  Result.append ("_");
  Result.append(const_cast< UmlArtifact& >(a_target).name ());
  Result.replace( QChar{'.'}, QChar{'_'} );
  return Result.upper ();

}

base_generator::base_generator() 
:  individual_obj_()
, other_target_type_()
, root_dir_( new pathcmp() )
, target_NAME_()
{}

base_generator::~base_generator() 
{}


} // namespace butter

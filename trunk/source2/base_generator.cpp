
#include "base_generator.h"
#include "compound_artifact.h"
#include "location.h"

namespace butter {

//DRIVER METHOD
//
//Create a series of build-system artifacts based at a_base location. The
//artifacts are inserted into a package at each location.
void base_generator::create_system() {
}

//Parse a_artifact's properties and description (iff a_not_doc) looking for 
//include directories  and linker options. Append what is found to a_hdr, 
//a_link and a_flag.
void base_generator::find_hdr_link()
{
}

//Find an artifact with a_name in a_loc. If not found then create and
//initialise the document.
//
//!!Warning!! returns nul iff a_loc.packages.empty
std::unique_ptr< compound_artifact > base_generator::get_artifact()
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

//The effect of this method is to convert the contents of a_list and a_addition
//into lists of tokens.  These lists are merged, removing duplicate items, and
//the new list is assigned back to a_list. Either list may be empty.
void base_generator::merge_string_list()
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

//This method finds the substring in a_desc between "${"a_section"}"
//and the next "${".  If no "${" is found then nothing is returned. Used
//to get style specific data from target artifact description fields.
QString base_generator::section()
{
static const QString marker_start_ ("${" + constants::section_prefix);
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

//Set the project's root directory
void base_generator::root_dir() {
  BUTTER_ENSURE (! a_path.path ().isEmpty ()
  , "<p><b>Program error:</b> Attempt to set project root directory to an empty path.</p>");
  root_dir_ = std::unique_ptr < pathcmp >(new pathcmp (a_path));
}

QString base_generator::build_target_NAME()
{
if( not package_name.isEmpty() )
{
  package_name.append( "_" );
  package_name.append( target_name );
}
else
{
  std::swap( package_name, target_name );
}
package_name.replace( QChar{'.'}, QChar{'_'} );
package_name.replace( QChar{'-'}, QChar{'_'} );
return package_name.upper();

}

base_generator::base_generator() 
:  individual_obj_()
, other_target_type_()
, root_dir_( new pathcmp() )
, target_NAME_()
{
}

base_generator::~base_generator() {
}

// Add necessary quoting to an input string.
//
// A user of the system may want to set a preprocessor 
// flag of [-DSOMETEXT="Help Text."] as a property value
// with the same text. The generators must be able to 
// quote/escape this text so that it is correctly passed
// to the final command.
QByteArray base_generator::quote_string()
{

}


} // namespace butter

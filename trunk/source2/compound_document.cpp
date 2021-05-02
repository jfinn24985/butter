
#include "compound_document.h"

// manual includes
#include "config.h"
#include "constants.h"
// end
namespace butter {

//Is there no user or value string set.
bool compound_document::document_element::isEmpty() const {
  return this->user.isEmpty() && this->value.isEmpty(); }

//Construct and initialise object from a_art
compound_document::compound_document() 
: end_phrase_(), start_phrase_()
, close_( butter::constants::close_label, {}, {} )
, date_( butter::constants::date_label, {}, {} )
, postscript_()
, preamble_( butter::constants::preamble_label, {}, {} )
, targets_()
, version_( butter::constants::version_label, {}, {} )
{}

//Get target at a given index.
//
//\pre idx < size
const compound_document::document_element & compound_document::at(unsigned int idx) const {
bool guard=false;
this->targets_.at( idx, &guard );
if( ! guard )
{
  throw std::out_of_range( "index out of range" );
}
return this->targets_.at( idx, &guard );

}

//Get target element with a_label.
//
//\pre has_target( a_label )
const compound_document::document_element & compound_document::at(QString a_label) const {
for( auto const& elem : this->targets_ )
{
  if( elem.label == a_label ) return elem;
}
BUTTER_ALWAYS( false, "Unable to find target with the given label" );
static document_element dummy;
return dummy; // Should never get here
}

QString compound_document::end_phrase() const {
  return this->end_phrase_; }

//Check for the existance of a target.
bool compound_document::has_target(QString a_label) const {
for( auto const& elem : this->targets_ )
{
  if( elem.label == a_label ) return true;
}
return false;
}

QString compound_document::start_phrase() const {
  return this->start_phrase_; }

//Get target element with a_label.
//
//If not has_target( a_label ) inserts and returns a new empty element
compound_document::document_element & compound_document::operator[](QString a_label) {
for( auto & elem : this->targets_ )
{
  if( elem.label == a_label ) return elem;
}
this->targets_.push_back( { a_label, {}, {} } );
return this->targets_.back();
}

void compound_document::end_phrase(QString val) {
  this->end_phrase_ = val; }

void compound_document::start_phrase(QString val) {
  this->start_phrase_ = val; }

//Merges the content of two compound documents.
//
//Merge process:
//
//* For any elements with the same label, append the other data to 
//  the element in this object.
//
//* Append any elements in the other object that are not in this
//  object.
//
//\pre ! start_phrase.empty && ! end_phrase.empty
//	&& start_phrase == other.start_phrase
//	&& end_phrase == other.end_phrase
void compound_document::merge(const compound_document & a_other) {
BUTTER_REQUIRE( ! this->start_phrase_.isEmpty(), "Can not merge document without phrases set." );
BUTTER_REQUIRE( ! this->end_phrase_.isEmpty(), "Can not merge document without phrases set." );
BUTTER_REQUIRE( this->end_phrase_ == a_other.end_phrase_, "Can not merge document with different phrases set." );
BUTTER_REQUIRE( this->end_phrase_ == a_other.end_phrase_, "Can not merge document with different phrases set." );
auto do_merge = []( document_element & targ_, const document_element & src_ )
{
  if ( ! src_.user.isEmpty() )
  {
    targ_.user.append( src_.user );
  }
  if ( ! src_.value.isEmpty() )
  {
    targ_.value.append( src_.value );
  }
};
do_merge( this->version_, a_other.version_ );
do_merge( this->date_, a_other.date_ );
do_merge( this->preamble_, a_other.preamble_ );
do_merge( this->close_, a_other.close_ );
for ( auto const& o_elem : a_other.targets_ )
{
  bool found = false;
  for ( auto & elem : this->targets_ )
  {
    if( o_elem.label == elem.label )
    {
      do_merge( elem, o_elem );
      found = true;
      break;
    }
  }
  if( ! found )
  {
    this->targets_.push_back( o_elem );
  }
}
if( ! a_other.postscript_.isEmpty() )
{
  this->postscript_.append( a_other.postscript_ );
}

}

//Reset object and read-in a new artifact.
//
//The version, date, preamble and close are read as-is. User data preceding
//targets are read and the sequence order of targets is maintained, but the 
//content of the target section is left blank.
//
//The method uses the start_phrase and end_phrase as regular expressions for
//finding the beginning and ending of autogenerated and user sections.
//
//\pre ! start_phrase.empty && ! end_phrase.empty

void compound_document::parse(QTextIStream & a_is) {
BUTTER_REQUIRE( ! this->start_phrase_.isEmpty(), "Can not parse document without phrases set." );
BUTTER_REQUIRE( ! this->end_phrase_.isEmpty(), "Can not parse document without phrases set." );
QString capture_;
const QString start_phrase{ this->start_phrase_ };
const QString end_phrase{ this->end_phrase_ };
int state_ = 0; // 0 = unlabelled, 1 = labelled
document_element * cache_ = NULL;
compound_document tmp;
for ( QString line_ = a_is.readLine();
  ! line_.isNull(); // Check for EOF
  line_ = a_is.readLine() )
{
  switch ( state_ )
  {
  case 0: // Unlabelled
  {
    if ( line_.startsWith( start_phrase ) )
    {
      if ( line_.contains( QString( butter::constants::version_label ) ) )
      {
        cache_ = &tmp.version_;
      }
      else if ( line_.contains( QString( butter::constants::date_label ) ) )
      {
        cache_ = &tmp.date_;
      }
      else if ( line_.contains( QString( butter::constants::preamble_label ) ) )
      {
        cache_ = &tmp.preamble_;
      }
      else if ( line_.contains( QString( butter::constants::close_label ) ) )
      {
        cache_ = &tmp.close_;
      }
      else if ( line_.contains( QString( butter::constants::target_label ) ) )
      {
        const QString label_ = line_.mid( line_.find( QString( butter::constants::target_label ) ) + butter::constants::target_label.length() ).simplifyWhiteSpace();
        cache_ = &tmp[ label_ ];
      }
      else
      {
        QString name_( "<p><b>Fatal Error:</b> An unknown section type (in line <b>" );
        name_.append( line_ );
        name_.append( "</b>) encountered while parsing a compound document.</p>" );
        throw std::runtime_error( name_.utf8().data() );
      }
      if ( ! capture_.isEmpty() )
      {
        cache_->user = capture_;
        capture_.setUnicode( 0, 0 );
      }
      state_ = 1;
    }
    else if ( line_.startsWith( end_phrase ) )
    {
      QString name_( "<p><b>Fatal Error:</b> Section end phrase (in line <b>" );
      name_.append( line_ );
      name_.append( "</b> encountered outside a section while parsing a compound document.</p>" );
      throw std::runtime_error( name_.utf8().data() );
    }
    else
    {
      capture_ += line_;
      capture_ += '\n';
    }
  }
  break;
  case 1: // In a section
  {
    if ( line_.startsWith( end_phrase ) )
    {
      if ( ! capture_.isEmpty() )
      {
        cache_->value = capture_;
        capture_.setUnicode( 0, 0 );
      }
      state_ = 0;
    }
    else if ( line_.startsWith( start_phrase ) )
    {
      throw std::runtime_error( "<p>Badly formed artifact with section start inside a section</p>" );
    }
    else
    {
      capture_ += line_;
      capture_ += '\n';
    }
  }
  break;
  default: // Error!
    throw std::runtime_error( "<p>Programming error: at a default switch case.</p>" );
    break;
  }
}
if ( ! capture_.isEmpty() )
{
  if ( NULL != cache_ ) // Have there been sections?
  {
    tmp.postscript_ = capture_;
  }
  else
  {
    tmp.preamble_.user = capture_;
  }
}
this->swap( tmp );

}

//Emptys auto-generated values for all targets, preamble and close 
//labelled sections, leaving any user supplied data and the version and 
//date elements unchanged.
void compound_document::reset() {
for( auto & elem : this->targets_ )
{
  elem.value.truncate( 0 );
}
if( ! this->preamble_.value.isEmpty() )
{
  this->preamble_.value.truncate( 0 );
}
if( ! this->close_.value.isEmpty() )
{
  this->close_.value.truncate( 0 );
}
}

//Output the parts into a_os
//
//The start and end phrase arguments provide bracket lines for each subsection
//of the input text.  These phrases are also used when parsing the text.
//
//\pre ! start_phrase.empty && ! end_phrase.empty

void compound_document::write(QTextOStream & a_os) const {
BUTTER_REQUIRE( ! this->start_phrase_.isEmpty(), "Can not write document without phrases set." );
BUTTER_REQUIRE( ! this->end_phrase_.isEmpty(), "Can not write document without phrases set." );
const QString start_phrase{ this->start_phrase_ };
const QString end_phrase{ this->end_phrase_ };
auto do_write = [&start_phrase, &end_phrase]( QTextStream & a_os_, const document_element & a_cache_)
{
  if ( ! a_cache_.user.isEmpty () )
  {
    a_os_ << a_cache_.user;
  }
  if ( ! a_cache_.value.isEmpty () )
  {
    a_os_ << start_phrase << a_cache_.label << "\n";
    a_os_ << a_cache_.value;
    a_os_ << end_phrase << a_cache_.label << "\n";
  }
};

do_write( a_os, this->version_ );
do_write( a_os, this->date_ );
do_write( a_os, this->preamble_ );
for ( auto const& elem : this->targets_ )
{
  document_element tmp{ elem };
  tmp.label = butter::constants::target_label + " " + tmp.label;
  do_write( a_os, tmp );
}
do_write( a_os, this->close_ );
if ( ! this->postscript_.isEmpty() )
{
  a_os << this->postscript_;
}
}


} // namespace butter


#include "butter/compound_document.h"
#include "butter/basic_style.h"
#include "butter/butter_constants.h"
#include "butter/utility.h"
#include "butter/style.h"

// Manual include
#include "butter/config.h"
namespace butter {

compound_document::compound_document() 
: close_( butter::butter_constants::close_label, {}, {} )
, date_( butter::butter_constants::date_label, {}, {} )
, end_()
, preamble_( butter::butter_constants::preamble_label, {}, {} )
, targets_()
, version_( butter::butter_constants::version_label, {}, {} )
{}



void compound_document::add_target(QString a_label, QString a_prevalue, QString a_value) 
{
BUTTER_REQUIRE( ! this->has_target( a_label ), "Cannot insert a second target with the same name" );
this->targets_.push_back( { a_label, a_prevalue, a_value } );

}

void compound_document::add_target_entry(QString a_label) 
{
BUTTER_REQUIRE( ! this->has_target( a_label ), "Cannot insert a second target with the same name" );
this->targets_.push_back( { a_label, {}, {} } );

}

const compound_document::element_t& compound_document::get_target(QString a_label) const 
{
for( auto const& elem : this->targets_ )
{
  if( elem.label == a_label ) return elem;
}
BUTTER_REQUIRE( false, "Unable to find target with the given label" );
static element_t dummy;
return dummy; // Should never get here

}

bool compound_document::has_target(QString a_label) const 
{
for( auto const& elem : this->targets_ )
{
  if( elem.label == a_label ) return true;
}
return false;

}

void compound_document::set_target(QString a_label, QString a_user, QString a_value) 
{
for( auto & elem : this->targets_ )
{
  if( elem.label == a_label )
  {
    elem.user = a_user;
    elem.value = a_value;
    return;
  }
}
BUTTER_REQUIRE( false, "Cannot set target value for non-existent target" );

}

void compound_document::set_target_user(QString a_label, QString a_preamble) 
{
for( auto & elem : this->targets_ )
{
  if( elem.label == a_label )
  {
    elem.user = a_preamble;
    return;
  }
}
BUTTER_REQUIRE( false, "Cannot set target value for non-existent target" );

}

void compound_document::set_target_value(QString a_label, QString a_value) 
{
for( auto & elem : this->targets_ )
{
  if( elem.label == a_label )
  {
    elem.value = a_value;
    return;
  }
}
BUTTER_REQUIRE( false, "Cannot set target value for non-existent target" );

}

const compound_document::element_t& compound_document::target_at(unsigned int idx) const 
{
bool guard=false;
this->targets_.at( idx, &guard );
if( ! guard )
{
  throw std::out_of_range( "index out of range" );
}
return this->targets_.at( idx, &guard );

}

void compound_document::merge(const compound_document & a_other) 
{
auto do_merge = []( element_t & targ_, const element_t & src_ )
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
if( ! a_other.end_.isEmpty() )
{
  this->end_.append( a_other.end_ );
}

}

void compound_document::parse(::QTextIStream & a_is, const basic_style & a_style) 
{
QString capture_;
int state_ = 0; // 0 = unlabelled, 1 = labelled
element_t * cache_ = NULL;
compound_document tmp;
for ( QString line_ = a_is.readLine();
  ! line_.isNull(); // Check for EOF
  line_ = a_is.readLine() )
{
  switch ( state_ )
  {
  case 0: // Unlabelled
  {
    if ( line_.startsWith( a_style.start_phrase() ) )
    {
      if ( line_.contains( QString( butter::butter_constants::version_label ) ) )
      {
        cache_ = &tmp.version_;
      }
      else if ( line_.contains( QString( butter::butter_constants::date_label ) ) )
      {
        cache_ = &tmp.date_;
      }
      else if ( line_.contains( QString( butter::butter_constants::preamble_label ) ) )
      {
        cache_ = &tmp.preamble_;
      }
      else if ( line_.contains( QString( butter::butter_constants::close_label ) ) )
      {
        cache_ = &tmp.close_;
      }
      else if ( line_.contains( QString( butter::butter_constants::target_label ) ) )
      {
        const QString label_ = line_.mid( line_.find( QString( butter::butter_constants::target_label ) ) + butter::butter_constants::target_label.length() ).simplifyWhiteSpace();
        tmp.add_target_entry( label_ );
        cache_ = &tmp.targets_[ tmp.size() - 1 ];
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
    else if ( line_.startsWith( a_style.end_phrase() ) )
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
    if ( line_.startsWith( a_style.end_phrase() ) )
    {
      if ( ! capture_.isEmpty() )
      {
        cache_->value = capture_;
        capture_.setUnicode( 0, 0 );
      }
      state_ = 0;
    }
    else if ( line_.startsWith( a_style.start_phrase() ) )
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
    tmp.end_ = capture_;
  }
  else
  {
    tmp.preamble_.user = capture_;
  }
}
this->swap( tmp );

}

void compound_document::reset() 
{
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

void compound_document::write(::QTextOStream & a_os, const basic_style & a_style) const 
{
auto do_write = []( QTextStream & a_os_, const element_t & a_cache_, const butter::basic_style & s_ )
{
  if ( ! a_cache_.user.isEmpty () )
  {
    a_os_ << a_cache_.user;
  }
  if ( ! a_cache_.value.isEmpty () )
  {
    a_os_ << s_.start_phrase() << a_cache_.label << s_.end_start_phrase() << "\n";
    a_os_ << a_cache_.value;
    a_os_ << s_.end_phrase() << a_cache_.label << s_.end_end_phrase() << "\n";
  }
};

do_write( a_os, this->version_, a_style );
do_write( a_os, this->date_, a_style );
do_write( a_os, this->preamble_, a_style );
for ( auto const& elem : this->targets_ )
{
  element_t tmp{ elem };
  tmp.label = butter::butter_constants::target_label + " " + tmp.label;
  do_write( a_os, tmp, a_style );
}
do_write( a_os, this->close_, a_style );
if ( ! this->end_.isEmpty() )
{
  a_os << this->end_;
}

}


} // namespace butter

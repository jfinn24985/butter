
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
//XX QString capture_;
//XX int state_ = 0; // 0 = unlabelled, 1 = labelled
//XX string_pair_t * cache_ = NULL;
//XX for ( QString line_ = a_is.readLine();
//XX   ! line_.isNull(); // Check for EOF
//XX   line_ = a_is.readLine() )
//XX {
//XX   switch ( state_ )
//XX   {
//XX   case 0: // Unlabelled
//XX   {
//XX     if ( line_.startsWith( style_.start_phrase() ) )
//XX     {
//XX       if ( line_.contains( QString( butter::butter_constants::version_label ) ) )
//XX       {
//XX         cache_ = &version;
//XX       }
//XX       else if ( line_.contains( QString( butter::butter_constants::date_label ) ) )
//XX       {
//XX         cache_ = &date;
//XX       }
//XX       else if ( line_.contains( QString( butter::butter_constants::preamble_label ) ) )
//XX       {
//XX         cache_ = &preamble;
//XX       }
//XX       else if ( line_.contains( QString( butter::butter_constants::close_label ) ) )
//XX       {
//XX         cache_ = &close;
//XX       }
//XX       else if ( line_.contains( QString( butter::butter_constants::target_label ) ) )
//XX       {
//XX         const QString label_ = line_.mid( line_.find( QString( butter::butter_constants::target_label ) ) + butter::butter_constants::target_label.length() ).simplifyWhiteSpace();
//XX         this->add_target_entry( label_ );
//XX         cache_ = this->targets_.find( label_ );
//XX       }
//XX       else
//XX       {
//XX         QString name_( "<p><b>Fatal Error:</b> An unknown section type (in line <b>" );
//XX         name_.append( line_ );
//XX         name_.append( "</b>) encountered while parsing a compound document.</p>" );
//XX         throw std::runtime_error( name_.utf8().data() );
//XX       }
//XX       if ( ! capture_.isEmpty() )
//XX       {
//XX         cache_->first = capture_;
//XX         capture_.setUnicode( 0, 0 );
//XX       }
//XX       state_ = 1;
//XX     }
//XX     else if ( line_.startsWith( style_.end_phrase() ) )
//XX     {
//XX       QString name_( "<p><b>Fatal Error:</b> Section end phrase (in line <b>" );
//XX       name_.append( line_ );
//XX       name_.append( "</b> encountered outside a section while parsing a compound document.</p>" );
//XX       throw std::runtime_error( name_.utf8().data() );
//XX     }
//XX     else
//XX     {
//XX       capture_ += line_;
//XX       capture_ += '\n';
//XX     }
//XX   }
//XX   break;
//XX   case 1: // In a section
//XX   {
//XX     if ( line_.startsWith( style_.end_phrase() ) )
//XX     {
//XX       if ( ! capture_.isEmpty() )
//XX       {
//XX         cache_->second = capture_;
//XX         capture_.setUnicode( 0, 0 );
//XX       }
//XX       state_ = 0;
//XX     }
//XX     else if ( line_.startsWith( style_.start_phrase() ) )
//XX     {
//XX       throw std::runtime_error( "<p>Badly formed artifact with section start inside a section</p>" );
//XX     }
//XX     else
//XX     {
//XX       capture_ += line_;
//XX       capture_ += '\n';
//XX     }
//XX   }
//XX   break;
//XX   default: // Error!
//XX     throw std::runtime_error( "<p>Programming error: at a default switch case.</p>" );
//XX     break;
//XX   }
//XX }
//XX if ( ! capture_.isEmpty() )
//XX {
//XX   if ( NULL != cache_ ) // Have there been sections?
//XX   {
//XX     end = capture_;
//XX   }
//XX   else
//XX   {
//XX     version.first = capture_;
//XX   }
//XX }

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

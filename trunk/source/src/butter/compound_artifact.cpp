
#include "butter/compound_artifact.h"
#include "butter/basic_style.h"
#include "butter/butter_constants.h"
#include "butter/utility.h"
#include "butter/style.h"
#include "bouml/UmlArtifact.h"
#include "butter/compound_document.h"

// Manual includes
#include <stdexcept>
#include "butter/config.h"
// --
namespace butter {

compound_artifact::compound_artifact(::UmlArtifact & a_art) 
: artifact_( a_art )
, doc_()
, style_( butter::style::get_style() )
{}

compound_artifact::~compound_artifact() 
{
// Only check if the document has been read.
if( nullptr != this->doc_.get() )
{
  // Compare to existing document
  QString desc_;
  const QString orig_desc_( this->artifact_.description () );
  {
    QTextOStream os_( &desc_ );
    this->doc_->write( os_, this->style_ );
  }
  if( 0 != orig_desc_.compare( desc_ ) )
  {
    // Document changed, update version and date
    this->doc_->set_version_value( this->style_.comment_text( butter_constants::BUTTER_VERSION ) );
    this->doc_->set_date_value( this->style_.comment_text( QString( QDateTime::currentDateTime().toString() ) ) );
    QString desc2_;
    {
      QTextOStream os2_( &desc2_ );
      this->doc_->write( os2_, this->style_ );
    }
    this->artifact_.set_Description( desc2_.utf8() );
  }
}

}

void compound_artifact::merge(::UmlArtifact & a_art) 
{
compound_document other;
#if QT_VERSION < 300L
QString tmp_( a_art.description() );
#else
const QString tmp_( a_art.description() );
#endif
QTextIStream is_( &tmp_ );
other.parse( is_, this->style_ );
this->document().merge( other );

}

compound_document & compound_artifact::document() 
{
if( ! this->doc_ )
{
  this->doc_.reset( new compound_document );
#if QT_VERSION < 300L
  QString tmp_( this->artifact_.description() );
#else
  const QString tmp_( this->artifact_.description() );
#endif
  QTextIStream is_( &tmp_ );
  this->doc_->parse( is_, this->style_ );
}
return *(this->doc_);

}


} // namespace butter

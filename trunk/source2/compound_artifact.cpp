
#include "compound_artifact.h"
#include "compound_document.h"
#include "basic_style.h"

namespace butter {

//Construct and initialise object from a_art
compound_artifact::compound_artifact() {
}

//Construct and initialise object from a_art
compound_artifact::compound_artifact() {
}

compound_artifact::~compound_artifact() {
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
    this->doc_->set_version_value( this->style_.comment_text( constants::BUTTER_VERSION ) );
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

//Merges the content of a_art into this artifact.
//
//Merge process:
//* Create temporary compound_artifact from a_art.
//* If any labelled sections exist in both artifacts then the content 
//from a_art is appended. If an unlabelled section exists before the
//labelled section then is is appended to any preceding unlabelled
//section or inserted if no existing labelled section exists.
//* If any sections remain then they are appended inorder to this 
//artifact. 
void compound_artifact::merge() {
}

// Access the artifact's description as a compound document.
compound_document compound_artifact::document() {
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


#include "UmlArtifact.h"
#include "utility.h"
#include "compound_document.h"

//If the compound document of this artifact has changed, update the description.
UmlArtifact::~UmlArtifact() {
// Only check if the document has been read.
if( nullptr != this->doc_.get() )
{
  // Compare to existing document
  QString desc_;
  const QString orig_desc_( this->description () );
  {
    QTextOStream os_( &desc_ );
    this->doc_->write( os_ );
  }
  if( 0 != orig_desc_.compare( desc_ ) )
  {
    // Document changed, update version and date
    this->doc_->version().value = constants::comment_text( constants::BUTTER_VERSION );
    this->doc_->date().value = constants::comment_text( QString( QDateTime::currentDateTime().toString() ) );
    QString desc2_;
    {
      QTextOStream os2_( &desc2_ );
      this->doc_->write( os2_, this->style_ );
    }
    this->set_Description( desc2_.utf8() );
  }
}

}

//If this artifact has stereotype "document" write to the corresponding file,
//if the text has changed.
//
//\throw : runtime_error if file can not be openned for reading or writing
//
//\arg full_path : path of directory containing this artifact
//\arg log : output for user
void UmlArtifact::write_out(const butter::enumerate_path & full_path, butter::base_log & log) {
  if (constants::document_stereotype == this->stereotype().data())
  {
    // Only write non-empty docs.
    if (! this->description().isEmpty())
    {
      QByteArray document_prop_value_;
      this->propertyValue(constants::butter_output_name(), document_prop_value_);
      if (document_prop_value_.isEmpty() || (document_prop_value_ != constants::nowrite()))
      {
        QString document_filepath_{ (full_path / this->name()).path() };
        if (! document_prop_value_.isEmpty())
        {
          document_filepath_ += "." + document_prop_value_;
        }
        bool result{ false };
        if(QFile::exists(document_filepath_.path()))
        {
          QFile iofile(a_path);
          if (! iofile.open(IO_ReadOnly))
          {
            QString msg("<b>Fatal Error:</b> Unable to open file for reading: " + document_filepath_.path());
            throw std::runtime_error(msg.toStdString());
          }
          QByteArray contents_{ iofile.readAll() };
          result = (contents_.size() == this->description().size()
                    && 0 == strncmp(contents_.constData(), this->description().constData(), contents_.size()));
        }
        if (! result)
        {
          log.trace(log.warn, "<p>Writing changed document : " + document_filepath_.path() + "</p>");
          QFile iofile(a_path);
          if (! iofile.open(IO_ReadOnly))
          {
            QString msg("<b>Fatal Error:</b> Unable to open file for writing: " + document_filepath_.path());
            throw std::runtime_error(msg.toStdString());
          }
          iofile.writeBlock(this->description(), this->description.size());
        }
        else
        {
          log.trace(log.info, "<p>Skipping unchanged document : " + document_filepath_.path() + "</p>");
        }
      }
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
void UmlArtifact::merge(UmlArtifact & art) {
  // check we have local doc or generate
  if( nullptr == this->doc_.get() )
  {
    BUTTER_REQUIRE( nullptr != art.doc_.get(), "Get document before merging." );
    this->document( art.doc_->start_phrase(), art.doc_->end_phrase() );
  }
  // check we have other doc or generate
  if( nullptr == art.doc_.get() )
  {
    art.document( this->doc_->start_phrase(), this->doc_->end_phrase() );
  }
  BUTTER_REQUIRE( nullptr != this->doc_.get() && nullptr != art.doc_.get(), "Unable to create compound documents from UmlArtifact object." );
  // merge
  this->doc_->merge( *art.doc_ );
}

// Access the artifact's description as a compound document.
butter::compound_document UmlArtifact::document(QString start_phrase, QString end_phrase) {
if( ! this->doc_ )
{
  this->doc_.reset( new compound_document );
#if QT_VERSION < 300L
  QString tmp_( this->artifact_.description() );
#else
  const QString tmp_( this->artifact_.description() );
#endif
  QTextIStream is_( &tmp_ );
  this->doc_->start_phrase( start_phrase );
  this->doc_->end_phrase( end_phrase );
  this->doc_->parse( is_ );
}
return *(this->doc_);
}


#ifndef BUTTER_COMPOUND_DOCUMENT_H
#define BUTTER_COMPOUND_DOCUMENT_H


#include <qstring.h>
#include <q3valuevector.h>
#include <qtextstream.h>

namespace butter {

//A compound document is used to manage documents that
//contain user and auto-generated content. The auto-generated
//content contains information about a build target, the
//user content is something manually added by the user,
//possibly build information for subsequent targets. When
//initially generated the targets will have the following
//order: ("version", "date", "preamble", [build targets]*,
//"close" {end}). The {end} target is a special unnamed
//pseudo-target that contains any user text added after the
//generated text. The user can manually reorder the build
//target sequence, though the order of "version", "date",
//"preamble" and "close" are fixed. The object maintains the
//information as a list of string pairs. The first element
//is any text manually added by the user and the second
//element is the auto generated text.
//
//NOTES:
//
//  * If the user supplies an artifact with the appropriate
//  name before a build generator is run, any text in the
//  document will be added between the "date" and "preamble"
//  sections.
//
//  * (auto-generted) parts are enclosed in "start_phrase_" /
//  "end_phrase_" markers that are used to parse the document
//  back into a compound document.
//
//  * (user) parts are any non-whitespace outside the
//  markers. This information is stored with the following
//  auto-generated part, except when there is no following
//  target in which case it is the {end} text.
//
//Standard build-file form is a compound_document with the
//following initial order
//
//[version] generator version information
//[date] generation date
//[preamble] ... (optional)
//[target:....]* ...
//[close] ...  (optional)
//{end}  (optional)
//

class compound_document {
  public:
    struct document_element {
        //The element's label
        
        QString label;

        //The element's user-added text
        
        QString user;

        //The element's generated text
        
        QString value;

        document_element() = default;
        document_element(QString a_label, QString a_user, QString a_value) : label( a_label )
                , user( a_user )
                , value( a_value )
                {};
        ~document_element() = default;
        document_element(document_element && source) : label( std::move( source.label ) )
                , user( std::move( source.user ) )
                , value( std::move( source.value ) )
                {};
        document_element(const document_element & source) : label( source.label )
                , user( source.user )
                , value( source.value )
                {};
        document_element & operator =(document_element source) {
                  this->swap( source );
                  return *this;
                };
        void swap(document_element & other) {
                  std::swap( label, other.label );
                  std::swap( user, other.user );
                  std::swap( value, other.value );
                };
        //Is there no user or value string set.
        bool isEmpty() const;

    };
    

  private:
    //Text that ends a delineated section
    QString end_phrase_;

    //Text that starts a delineated section
    QString start_phrase_;

    //The close texts (optional)
    
    document_element close_;

    //The date texts
    
    document_element date_;

    //Any user text at the end of the document.
    
    QString postscript_;

    //The preamble texts
    
    document_element preamble_;

    //The artifact's target parts.  
    //
    //The order these are read in and written out is meaningful, therefore we use a vector instead of a map.
    
    Q3ValueVector<document_element> targets_;

    //The version texts.
    
    document_element version_;


  public:
    //Construct and initialise object from a_art
    compound_document();
    ~compound_document() = default;
    inline compound_document(const compound_document & source)
        : end_phrase_( source.end_phrase_ ), start_phrase_( source.start_phrase_ )
        , close_( source.close_ )
        , date_( source.date_ )
        , postscript_( source.postscript_ )
        , preamble_( source.preamble_ )
        , targets_( source.targets_ )
        , version_( source.version_ )
        {};
    compound_document(compound_document && source)
        : end_phrase_( std::move( source.end_phrase_ ) )
        , start_phrase_( std::move( source.start_phrase_ ) )
        , close_( std::move( source.close_ ) )
        , date_( std::move( source.date_ ) )
        , postscript_( std::move( source.postscript_ ) )
        , preamble_( std::move( source.preamble_ ) )
        , targets_( std::move( source.targets_ ) )
        , version_( std::move( source.version_ ) )
        {};
    //no copy
    void swap(compound_document & source) noexcept {
          std::swap( end_phrase_, source.end_phrase_ );
          std::swap( start_phrase_, source.start_phrase_ );
          std::swap( close_, source.close_ );
          std::swap( date_, source.date_ );
          std::swap( postscript_, source.postscript_ );
          std::swap( preamble_, source.preamble_ );
          std::swap( targets_, source.targets_ );
          std::swap( version_, source.version_ );
        };
    compound_document & operator =(compound_document source) {
          this->swap( source );
          return *this;
        };
    //Get target at a given index.
    //
    //\pre idx < size
    const document_element & at(unsigned int idx) const;
    //Get target element with a_label.
    //
    //\pre has_target( a_label )
    const document_element & at(QString a_label) const;
    //Get target at position idx
    const document_element & operator[](unsigned int idx) const {
       return this->targets_[ idx ];
    }
    

    //Get the document close element (precedes end_text).
    const document_element & close() const {
       return this->close_;
    }
    //Get the date element.
    const document_element & date() const {
       return this->date_;
    }
    bool empty() const {
       return this->targets_.isEmpty();
    }
    QString end_phrase() const;

    //Get any user supplied information at the end of the document
    const QString & postscript() const {
      return this->postscript_;
    }
    //Check for the existance of a target.
    bool has_target(QString a_label) const;
    //Get the preamble element.
    const document_element & preamble() const {
       return this->preamble_;
    }
    size_t size() const {
          return this->targets_.size();
        };
    QString start_phrase() const;

    //Get the version element.
    const document_element & version() const {
       return this->version_;
    }
    //Get target element with a_label.
    //
    //If not has_target( a_label ) inserts and returns a new empty element
    document_element & operator[](QString a_label);
    //Get the document close element (precedes end_text).
    document_element & close() {
       return this->close_;
    }
    //Get the date element.
    document_element & date() {
       return this->date_;
    }
    void end_phrase(QString val);

    //Get any user supplied information at the end of the document
    QString & postscript() {
      return this->postscript_;
    }
    //Get the preamble element.
    document_element & preamble() {
       return this->preamble_;
    }
    void start_phrase(QString val);

    //Get the version element.
    document_element & version() {
       return this->version_;
    }
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
    void merge(const compound_document & a_other);
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
    
    void parse(QTextIStream & a_is);
    //Emptys auto-generated values for all targets, preamble and close 
    //labelled sections, leaving any user supplied data and the version and 
    //date elements unchanged.
    void reset();
    //Output the parts into a_os
    //
    //The start and end phrase arguments provide bracket lines for each subsection
    //of the input text.  These phrases are also used when parsing the text.
    //
    //\pre ! start_phrase.empty && ! end_phrase.empty
    
    void write(QTextOStream & a_os) const;

};

} // namespace butter
#endif

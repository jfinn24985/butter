#ifndef BUTTER_COMPOUND_DOCUMENT_HPP
#define BUTTER_COMPOUND_DOCUMENT_HPP


#include <qstring.h>
#include <qvaluevector.h>
#include <qtextstream.h>


namespace butter { class basic_style; } 
namespace butter { class butter_constants; } 
namespace butter { class pathcmp; } 
namespace butter { class style; } 

namespace butter {

/**
 * A compound document is used to manage documents that
 * contain user and auto-generated content. The auto-generated
 * content contains information about a build target, the
 * user content is something manually added by the user,
 * possibly build information for subsequent targets. When
 * initially generated the targets will have the following
 * order: ("version", "date", "preamble", [build targets]*,
 * "close" {end}). The {end} target is a special unnamed
 * pseudo-target that contains any user text added after the
 * generated text. The user can manually reorder the build
 * target sequence, though the order of "version", "date",
 * "preamble" and "close" are fixed. The object maintains the
 * information as a list of string pairs. The first element
 * is any text manually added by the user and the second
 * element is the auto generated text.
 * 
 * NOTES:
 * 
 *   * If the user supplies an artifact with the appropriate
 *   name before a build generator is run, any text in the
 *   document will be added between the "date" and "preamble"
 *   sections.
 * 
 *   * (auto-generted) parts are enclosed in "start_phrase_" /
 *   "end_phrase_" markers that are used to parse the document
 *   back into a compound document.
 * 
 *   * (user) parts are any non-whitespace outside the
 *   markers. This information is stored with the following
 *   auto-generated part, except when there is no following
 *   target in which case it is the {end} text.
 * 
 * Standard build-file form is a compound_document with the
 * following initial order
 * 
 * [version] generator version information
 * [date] generation date
 * [preamble] ... (optional)
 * [target:....]* ...
 * [close] ...  (optional)
 * {end}  (optional)
 * 
 * 
 */
class compound_document
{
  public:
    struct element_t
    {
        /**
         * The element's label
         */
        QString label;

        /**
         * The element's user-added text
         */
        QString user;

        /**
         * The element's generated text
         */
        QString value;

        element_t() = default;

        element_t(QString a_label, QString a_user, QString a_value)
        : label( a_label )
        , user( a_user )
        , value( a_value )
        {}

        ~element_t() = default;

        element_t(element_t && source)
        : label( std::move( source.label ) )
        , user( std::move( source.user ) )
        , value( std::move( source.value ) )
        {}
        

        element_t(const element_t & source)
        : label( source.label )
        , user( source.user )
        , value( source.value )
        {}
        element_t & operator=(element_t source)
        {
          this->swap( source );
          return *this;
        }

        void swap(element_t & other)
        {
          std::swap( label, other.label );
          std::swap( user, other.user );
          std::swap( value, other.value );
        }


    };
    
    

  private:
    /**
     * The close texts (optional)
     */
    element_t close_;

    /**
     * The date texts
     */
    element_t date_;

    /**
     * Any user text at the end of the document.
     */
    QString end_;

    /**
     * The preamble texts
     */
    element_t preamble_;

    /**
     * The artifact's target parts.
     */
    QValueVector< element_t > targets_;

    /**
     * The version texts.
     */
    element_t version_;


  public:
    /**
     * Construct and initialise object from a_art
     */
    compound_document();

    ~compound_document() {}

    compound_document(const compound_document & source)
    : close_( source.close_ )
    , date_( source.date_ )
    , end_( source.end_ )
    , preamble_( source.preamble_ )
    , targets_( source.targets_ )
    , version_( source.version_ )
    {}
    compound_document(compound_document && source)
    : close_( std::move( source.close_ ) )
    , date_( std::move( source.date_ ) )
    , end_( std::move( source.end_ ) )
    , preamble_( std::move( source.preamble_ ) )
    , targets_( std::move( source.targets_ ) )
    , version_( std::move( source.version_ ) )
    {}

    /**
     * no copy
     */
    void swap(compound_document & source)
    {
      std::swap( close_, source.close_ );
      std::swap( date_, source.date_ );
      std::swap( end_, source.end_ );
      std::swap( preamble_, source.preamble_ );
      std::swap( targets_, source.targets_ );
      std::swap( version_, source.version_ );
    }

    compound_document & operator=(compound_document source)
    {
      this->swap( source );
      return *this;
    }
    /**
     * Insert a new target into the document, with the
     * given user and value text.
     * 
     * \pre not has_target( a_label )
     */
    void add_target(QString a_label, QString a_prevalue, QString a_value);

    /**
     * Insert a new blank target into the document.
     * 
     * \pre not has_target( a_label )
     */
    void add_target_entry(QString a_label);

    /**
     * Append to value data of the close element.
     */
    void append_close_value(QString a_value)
    {
      this->close_.value.append( a_value );
    }

    /**
     * Append to value data of the preamble element.
     */
    void append_preamble_value(QString a_value)
    {
      this->preamble_.value.append( a_value );
    }

    /**
     * Append to value data of an existing target.
     * 
     * \pre has_target( a_label )
     */
    void append_target_value(QString a_label, QString a_value);

    /**
     * Get the document close element (precedes end_text).
     */
    const element_t& close() const
    {
      return this->close_;
    }

    /**
     * Get the date element.
     */
    const element_t& date() const
    {
      return this->date_;
    }

    bool empty() const
    {
      return this->targets_.isEmpty();
    }

    /**
     * Get any user supplied information at the end of the document
     */
    QString end_text() const
    {
      return this->end_;
    }

    /**
     * Get target element with a_label.
     * 
     * \pre has_target( a_label ) [or return an empty element]
     */
    const element_t& get_target(QString a_label) const;

    /**
     * Check for the existance of a target.
     */
    bool has_target(QString a_label) const;

    /**
     * Get the preamble element.
     */
    const element_t& preamble() const
    {
      return this->preamble_;
    }

    /**
     * Set value data of the close element.
     */
    void set_close_user(QString a_value)
    {
      std::swap( this->close_.user, a_value );
    }

    /**
     * Set value data of the close element.
     */
    void set_close_value(QString a_value)
    {
      std::swap( this->close_.value, a_value );
    }

    /**
     * Set value data of the close element.
     */
    void set_date_user(QString a_value)
    {
      std::swap( this->date_.user, a_value );
    }

    /**
     * Set value data of the close element.
     */
    void set_date_value(QString a_value)
    {
      std::swap( this->date_.value, a_value );
    }

    /**
     * Set text at the end fo the document
     */
    void set_end_text(QString a_value)
    {
      std::swap( this->end_, a_value );
    }

    /**
     * Set value data of the close element.
     */
    void set_preamble_user(QString a_value)
    {
      std::swap( this->preamble_.user, a_value );
    }

    /**
     * Set value data of the preamble element.
     */
    void set_preamble_value(QString a_value)
    {
      std::swap( this->preamble_.value, a_value );
    }

    /**
     * Set element data of an existing target.
     * 
     * \pre has_target( a_label )
     */
    void set_target(QString a_label, QString a_user, QString a_value);

    /**
     * Set user text of an existing target.
     * 
     * \pre has_target( a_label )
     */
    void set_target_user(QString a_label, QString a_preamble);

    /**
     * Set element data of an existing target.
     * 
     * \pre has_target( a_label )
     */
    void set_target_value(QString a_label, QString a_value);

    /**
     * Set value data of the close element.
     */
    void set_version_user(QString a_value)
    {
      std::swap( this->version_.user, a_value );
    }

    /**
     * Set value data of the close element.
     */
    void set_version_value(QString a_value)
    {
      std::swap( this->version_.value, a_value );
    }

    /**
     * Get target at a given index.
     * 
     * \pre idx < size
     */
    const element_t& target_at(unsigned int idx) const;

    size_t size() const
    {
      return this->targets_.size();
    }

    /**
     * Get the version element.
     */
    const element_t& version() const
    {
      return this->version_;
    }

    /**
     * Merges the content of two compound documents.
     * 
     * Merge process:
     * 
     * * For any elements with the same label, append the other data to 
     *   the element in this object.
     * 
     * * Append any elements in the other object that are not in this
     *   object.
     */
    void merge(const compound_document & a_other);

    /**
     * Reset object and read-in a new artifact.
     * 
     * The version, date, preamble and close are read as-is. User data preceding
     * targets are read and the sequence order of targets is maintained, but the 
     * content of the target section is left blank.
     */
    void parse(::QTextIStream & a_is, const basic_style & a_style);

    /**
     * Emptys auto-generated values for all targets, preamble and close 
     * labelled sections, leaving any user supplied data.
     */
    void reset();

    /**
     * Output the parts into a_os
     */
    void write(::QTextOStream & a_os, const basic_style & a_style) const;


};


} // namespace butter
#endif

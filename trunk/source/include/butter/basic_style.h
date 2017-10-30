#ifndef BUTTER_BASIC_STYLE_HPP
#define BUTTER_BASIC_STYLE_HPP


#include <memory>
#include <qstring.h>

namespace butter { class base_generator; } 

namespace butter {

/**
 * POD type for storing basic information about compound_document elements
 */
class basic_style
{
  public:
    typedef std::unique_ptr< base_generator > (*write_factory_fn_t)();


  private:
    /**
     * beginning of comment string
     */
    QString comment_string_;

    /**
     * ending of comment string
     */
    QString end_comment_string_;

    /**
     * A string indicating the end of a labelled part.
     * 
     * The actual end-phrase will be
     * end_phrase_ () end_end_phrase_ or "end_phrase_ () {label} end_end_phrase_" where label is
     * the name of the part. Note the "{" "}" are part of phrase.
     */
    QString end_phrase_;

    /**
     * A string denoting the end-of-line for  the ending point of a part.
     */
    QString end_end_phrase_;

    /**
     * A string denoting the starting point of a part.
     * 
     * The actual start-phrase will be
     * "start_phrase_ {label} end_start_phrase_" where label is the name of the part.
     * Note the "{" "}" are part of phrase.
     */
    QString start_phrase_;

    /**
     * A string denoting the end-of-line for  the starting point of a part.
     */
    QString end_start_phrase_;

    /**
     * The name of the current style.
     */
    QString name_;

    /**
     * Method to create base_generator object for this style.
     */
    write_factory_fn_t factory_;

    basic_style() = delete;

  public:
    basic_style(QString a_com_start, QString a_com_end, QString a_end_phr, QString a_end_end, QString a_start_phr, QString a_start_end, QString a_name, const write_factory_fn_t & a_factory)
    : comment_string_(a_com_start)
    , end_comment_string_(a_com_end)
    , end_phrase_(a_end_phr)
    , end_end_phrase_(a_end_end)
    , start_phrase_(a_start_phr)
    , end_start_phrase_(a_start_end)
    , name_(a_name)
    , factory_(a_factory)
    {}


  private:
    basic_style(const basic_style & source) = delete;
    basic_style(basic_style && source) = delete;

  public:
    virtual ~basic_style() = default;


  private:
    basic_style & operator=(basic_style source) = delete;


  public:
    QString const& comment_string() const
    {
      return this->comment_string_;
    }

    QString const& end_comment_string() const
    {
      return this->end_comment_string_;
    }

    QString const& end_phrase() const
    {
      return this->end_phrase_;
    }

    QString const& end_end_phrase() const
    {
      return this->end_end_phrase_;
    }

    QString const& end_start_phrase() const
    {
      return this->end_start_phrase_;
    }

    QString const& name() const
    {
      return this->name_;
    }

    QString const& start_phrase() const
    {
      return this->start_phrase_;
    }

    /**
     * Transcribe the given text into a comment in the current
     * build system.  In general this prepends comment indicators
     * to each line of a_text and adds a terminal newline if
     * a_text does not end in a newline.
     */
    QString comment_text(QString a_text) const;

    /**
     * Create a build generator appropriate for the current style.
     */
    std::unique_ptr< base_generator > create_writer() const;


};


} // namespace butter
#endif

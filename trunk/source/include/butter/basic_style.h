#ifndef BUTTER_BASIC_STYLE_HPP
#define BUTTER_BASIC_STYLE_HPP


#include <memory>
#include <qstring.h>

namespace butter { class base_generator; } 

namespace butter {

/**
 * POD type for storing basic information about compound_document elements
 */
struct basic_style
{
    typedef std::auto_ptr< base_generator > (*write_factory_fn_t)();

    /**
     * beginning of comment string
     */
    const QString comment_string;

    /**
     * ending of comment string
     */
    const QString end_comment_string;

    /**
     * A string indicating the end of a labelled part.
     * 
     * The actual end-phrase will be
     * end_phrase () end_end_phrase or "end_phrase () {label} end_end_phrase" where label is
     * the name of the part. Note the "{" "}" are part of phrase.
     */
    const QString end_phrase;

    /**
     * A string denoting the end-of-line for  the ending point of a part.
     */
    const QString end_end_phrase;

    /**
     * A string denoting the starting point of a part.
     * 
     * The actual start-phrase will be
     * "start_phrase {label} end_start_phrase" where label is the name of the part.
     * Note the "{" "}" are part of phrase.
     */
    const QString start_phrase;

    /**
     * A string denoting the end-of-line for  the starting point of a part.
     */
    const QString end_start_phrase;

    /**
     * The name of the current style.
     */
    const QString name;

    /**
     * Method to create base_generator object for this style.
     */
    write_factory_fn_t factory;

    basic_style(QString a_com_start, QString a_com_end, QString a_end_phr, QString a_end_end, QString a_start_phr, QString a_start_end, QString a_name, const write_factory_fn_t & a_factory)
    : comment_string(a_com_start)
    , end_comment_string(a_com_end)
    , end_phrase(a_end_phr)
    , end_end_phrase(a_end_end)
    , start_phrase(a_start_phr)
    , end_start_phrase(a_start_end)
    , name(a_name)
    , factory(a_factory)
    {}

    basic_style(const basic_style & source)
    : comment_string(source.comment_string)
    , end_comment_string(source.end_comment_string)
    , end_phrase(source.end_phrase)
    , end_end_phrase(source.end_end_phrase)
    , start_phrase(source.start_phrase)
    , end_start_phrase(source.end_start_phrase)
    , name(source.name)
    , factory(source.factory)
    {}
    /**
     * Prepend comment indicators to each line of a_text. Also adds a terminal newline if
     * a_text does not end in a newline.
     */
    QString comment_text(QString a_text) const;

    /**
     * Create a build generator appropriate for the current style.
     */
    std::auto_ptr< base_generator > create_writer() const;


};


} // namespace butter
#endif

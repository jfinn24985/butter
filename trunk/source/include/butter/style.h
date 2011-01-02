#ifndef BUTTER_STYLE_HPP
#define BUTTER_STYLE_HPP


#include <qstring.h>
#include <memory>

#include "butter/config.h"
namespace butter { class bjam_generator; } 
namespace butter { class cmake_generator; } 
namespace butter { class gmake_generator; } 
namespace butter { class log; } 
namespace butter { class jam_generator; } 
namespace butter { class base_generator; } 

namespace butter {

/**
 * POD type for storing basic information about compound_document elements
 */
struct basic_style
{
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

    basic_style(QString a_com_start, QString a_com_end, QString a_end_phr, QString a_end_end, QString a_start_phr, QString a_start_end, QString a_name)
    : comment_string(a_com_start)
    , end_comment_string(a_com_end)
    , end_phrase(a_end_phr)
    , end_end_phrase(a_end_end)
    , start_phrase(a_start_phr)
    , end_start_phrase(a_start_end)
    , name(a_name)
    {}

    /**
     * Prepend comment indicators to each line of a_text. Also adds a terminal newline if
     * a_text does not end in a newline.
     */
    QString comment_text(QString a_text) const;


};

/**
 * A container object for meta-style related information. This includes a 
 * factory method for creating style specific base_generators.
 * 
 * People implementing new build-system styles need to edit set_style 
 * to add the new base_generator derived class to the system.
 */
class style : public basic_style
{
  public:
    typedef std::auto_ptr< base_generator > (*write_factory_fn_t)();


  private:
    /**
     * Method to create base_generator object for this style.
     */
    write_factory_fn_t factory_;

    /**
     * Singleton object.
     */
    static std::auto_ptr< style > style_;


  public:
    /**
     * Create a build generator appropriate for the current style.
     */
    std::auto_ptr< base_generator > create_writer() const;

    /**
     * Get the Singleton Style object. set_style must have been called
     * prior to calling this method.
     */
    static const style& get_style();

    /**
     * Set the style parameters.
     */
    static void set_style(QString a_style = QString());

    style(QString a_comment, QString a_end, QString a_start, QString a_name, write_factory_fn_t a_factory);

    ~style();


  private:
    /**
     * no copy
     */
    style(const style & source);

    /**
     * no assign
     */
    style & operator=(const style & source);


};


} // namespace butter
#endif

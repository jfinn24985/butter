#ifndef BUTTER_STYLE_HPP
#define BUTTER_STYLE_HPP


#include "butter/basic_style.h"
#include <memory>
#include <qstring.h>

namespace butter { class log; } 

namespace butter {

/**
 * A container object for meta-style related information. This includes a 
 * factory method for creating style specific base_generators.
 * 
 * People implementing new build-system styles need to edit set_style 
 * to add the new base_generator derived class to the system.
 */
class style : public basic_style
{
  private:
    /**
     * Singleton object.
     */
    static std::auto_ptr< style > style_;


  public:
    /**
     * Get the Singleton Style object. set_style must have been called
     * prior to calling this method.
     */
    static const style& get_style();

    /**
     * Set the style parameters.
     */
    static void set_style(QString a_style = QString());

    style(QString a_comment, QString a_end, QString a_start, QString a_name, basic_style::write_factory_fn_t a_factory);

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

    /**
     * create from parent class
     */
    style(const basic_style & source)
    : basic_style (source)
    {}


};


} // namespace butter
#endif

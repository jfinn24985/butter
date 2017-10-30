#ifndef BUTTER_STYLE_HPP
#define BUTTER_STYLE_HPP


#include <qstring.h>

namespace butter { class bjam_generator; } 
namespace butter { class cmake_generator; } 
namespace butter { class gmake_generator; } 
namespace butter { class jam_generator; } 
namespace butter { class log; } 
namespace butter { class basic_style; } 

namespace butter {

/**
 * A Singleton container object for meta-style related information. This includes a 
 * factory method for creating style specific base_generators.
 * 
 * People implementing new build-system styles need to edit set_style 
 * to add a basic_style object for the new derived base_generator.
 */
class style
{
  private:
    /**
     * Singleton object.
     */
    static const basic_style * style_;


  public:
    /**
     * Get the Singleton Style object. set_style must have been called
     * prior to calling this method.
     */
    static const basic_style& get_style();

    /**
     * Set the style parameters.
     */
    static void set_style(QString a_style = QString());


  private:
    style() = delete;


  public:
    virtual ~style() {}


  private:
    /**
     * no copy
     */
    style(const style & source) = delete;

    /**
     * no assign
     */
    style & operator=(const style & source) = delete;

    /**
     * create from parent class
     */
    style(const basic_style & source) = delete;


};


} // namespace butter
#endif

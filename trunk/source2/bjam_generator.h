#ifndef BUTTER_BJAM_GENERATOR_H
#define BUTTER_BJAM_GENERATOR_H


#include "generator.h"
#include "basic_style.h"
#include <qstring.h>
#include <memory>
#include <qtextstream.h>

namespace butter { class base_generator; } 
namespace butter { class compound_artifact; } 
namespace butter { class location; } 

namespace butter {

//Build file generator for boost-jam.
//
//This is the build system of choice for C++-only projects that use
//the Boost library.
class bjam_generator : public generator<bjam_generator> {
  template <class derived> friend class generator;

  public:
    static const basic_style style;

    //The default leaf filename for the current style
    
    static const QString build_file_name;

    //The default leaf filename for the current style
    
    static const QString build_file_sysname;

    //The name of the project rules file.
    
    static const QString rules_name;

    //This is the default contents of a the rules file (local.jam) If a 
    //document artifact with name 'local.jam' is not present when 
    //butter is executed with \@style='boost' then one will be created
    //using this string.
    
    static const char * default_rules[];


  private:
    //Is this an alternate/other library target?
    
    bool is_other_;

    //This name of the project
    
    QString project_name_;

    inline bjam_generator() : is_other_ ()
        , project_name_ ()
        {};

  public:
    inline ~bjam_generator() {};

  private:
    //no copy
    bjam_generator(const bjam_generator & ) = delete;
    //no copy
    bjam_generator(bjam_generator && source) = delete;
    //no assign
    bjam_generator & operator =(const bjam_generator & );

  public:
    //Create bjam generator object.s
    static std::unique_ptr< base_generator > create();

  private:
    //This method scans a_target for its direct sources and
    //direct and indirect dependencies. 
    void assoc_library();

    QTextOStream & a_os;

    //This method scans a_target for its direct sources and
    //direct and indirect dependencies. 
    void assoc_source();

    //Write the Jamfile descent links for a_location.
    void descendent_link(compound_artifact & a_art, compound_artifact & a_sys, const location & a_loc);
    //This method scans a_target for its direct sources and
    //direct and indirect dependencies. 
    void end_target();

    //Write the external a_target at a_location. 
    //
    //* searches system_artifact_ for an existing target
    //* new target sections are add to system_artifact_
    void external_target();

    //Write extra details to the top-level build file.
    
    void initialise();

    //Create an install target.
    void install_target();

    //This method scans a_target for its direct sources and
    //direct and indirect dependencies. 
    void start_target();

};

} // namespace butter
#endif

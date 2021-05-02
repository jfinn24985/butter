#ifndef BUTTER_JAM_GENERATOR_H
#define BUTTER_JAM_GENERATOR_H


#include "generator.h"
#include "basic_style.h"
#include <qstring.h>
#include <memory>
#include <qtextstream.h>

namespace butter { class base_generator; } 
namespace butter { class compound_artifact; } 
namespace butter { class location; } 

namespace butter {

//Build file generator for standard jam.
//
//This is the build system generator of choice as is supports the widest range of
//build options.
//
//\cond XDOC
//<style style="make">
//<limitation>The <code>standard</code> jam tool does not provide
//a shared library target. Providing a shared library for this style is on 
//the <link target="todo">TODO list</link>.</limitation>
//\endcond XDOC
class jam_generator : public generator<jam_generator> {
  template <class derived> friend class generator;

  public:
    static const basic_style style;

    static const QString build_file_name;

    static const QString build_file_sysname;


  private:
    //A template Jamrules.
    
    static const char * default_rules[];


  public:
    //The name of the jam rules document.
    
    static const QString rules_name;


  private:
    //Text for the a main targets associated sub-targets
    
    QString lib_defn_;

    //Jam "Grist" of the current target location. Set in initialisation.
    
    QString grist_;

    //The full target name for the currently processing target (defined in start_target)
    
    QString qualified_target_name_;

    //Main ctor, takes top-level a_project.
    //
    //\pre a_project.parent = nul
    jam_generator();
    //no copy
    jam_generator(const jam_generator & ) = delete;
    //no copy
    jam_generator(jam_generator && source) = delete;
    //no assign
    jam_generator & operator =(const jam_generator & ) = delete;

  public:
    inline ~jam_generator() {};
    //Create bjam generator object.s
    static std::unique_ptr< base_generator > create();

  private:
    //** This method a library association to the current target entry for a_target.
    //
    //Responsibilites
    //- Properties
    // - associated includes, ldflags, cflags
    void assoc_library();

    QTextOStream & a_os;

    //** This method sets up object for creating a new target entry for a_target.
    //
    //Responsibilites
    //- Properties
    // - doc/source: includes, ldflags, cflags
    // - compiler
    //- Other
    // - set compilation for individual 
    void assoc_source();


  public:
    //Check a_source for butter properties and add information to a_os.
    //a_is_source is true of a_source stereotype is "source" and false
    //if stereotype is "document".
    void check_properties();


  private:
    //Build the inter-buildfile link from this artifact/location to its parent and 
    //vice-versa. This is called just before the artifact is closed so has access 
    //to the (almost) complete content.
    void descendent_link(compound_artifact & a_art, compound_artifact & a_sys, const location & a_loc);
    //** This method finalises the target entry for a_target.
    void end_target();

    //In this style external targets are not used, external library 
    //data is written in local-targets directly where it is 
    //referenced.
    //
    //Responsibility
    //- Properties
    // - project (defines external)
    // - buildfile
    //- Other
    // - external target
    void external_target();

    //Write extra details to the top-level build file.
    //
    //Responsibilities:
    //- Property handling
    // - build-dir
    // - project: flags, include, ldflags, (library) type
    // - style
    // - version
    void initialise();

    //** Create an install target.
    //
    //Responsibilites
    //- Properties
    // - install (library, executable and document)
    void install_target();

    //** This method sets up object for creating a new target entry for a_target.
    //
    //Responsibility
    //- Properties
    // - compiler
    // - include, ldflags, cflags
    // - buildfile
    // - type (library)
    //- Other
    // - shared lib
    // - static lib
    // - executable
    // - non-standard target
    void start_target();

};

} // namespace butter
#endif

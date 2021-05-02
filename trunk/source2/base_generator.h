#ifndef BUTTER_BASE_GENERATOR_H
#define BUTTER_BASE_GENERATOR_H


#include <qstring.h>
#include <memory>
#include "utility.h"
#include <qbytearray.h>

namespace butter { class compound_artifact; } 
namespace butter { class location; } 

namespace butter {

//The build generator derived classes are responsible for creating a set of build system artifacts
//in a particular style.  These are inserted into the Uml Project where another class is 
//responsible for writing them to the file-system.
//
//The main action takes place in the create_system method.  The generator template class contains
//a definitive implementation of this method that attempts to uniformly support all features of the
//program, while calling derived class methods to implement the features for each style. The
//Curiously Recurring Template Idiom is used to avoid virtual methods calls for the style specific 
//functions.
//
//Actions
//* traverses a location tree examining packages
//* creates a build-file artifact for all locations with targets
//* links all the build-file artifacts together to form a build-system

class base_generator {
  public:
    enum install_type {
      bin =  0,
      file =  1,
      lib =  2,
      man =  3//File is a man page

    

    };

    enum target_type {
      executable =  0,
      static_library =  1,
      shared_library =  2,
      other =  3//Target is an "other" library type.

    

    };


  protected:
    //Extra description for objects individually built from source
    
    QString individual_obj_;

    //An "other" target library type name.  This is the name that is used for the
    //label/converted for the build rule.  See individual styles to see how this
    //is used and what requirements there are for supporting it in each style.
    
    QString other_target_type_;


  private:
    //The root directory of the project.
    
    std::unique_ptr<enumerate_path> root_dir_;

    //The version of the current target name to use with variables.
    
    QString target_NAME_;


  public:
    //DRIVER METHOD
    //
    //Create a series of build-system artifacts based at a_base location. The
    //artifacts are inserted into a package at each location.
    virtual void create_system();


  protected:
    //Parse a_artifact's properties and description (iff a_not_doc) looking for 
    //include directories  and linker options. Append what is found to a_hdr, 
    //a_link and a_flag.
    static void find_hdr_link();

    QString & a_hdr;

    //Find an artifact with a_name in a_loc. If not found then create and
    //initialise the document.
    //
    //!!Warning!! returns nul iff a_loc.packages.empty
    static std::unique_ptr< compound_artifact > get_artifact(location & a_loc, QString a_name);

  public:
    //The effect of this method is to convert the contents of a_list and a_addition
    //into lists of tokens.  These lists are merged, removing duplicate items, and
    //the new list is assigned back to a_list. Either list may be empty.
    static void merge_string_list(QString & a_list, QString a_addition);
    //This method finds the substring in a_desc between "${"a_section"}"
    //and the next "${".  If no "${" is found then nothing is returned. Used
    //to get style specific data from target artifact description fields.
    static QString section(QString a_section, QString a_desc);
    //Get the projects root directory (if set)
    inline const enumerate_path & root_dir() const {
          return *root_dir_.get ();
        };
    //Set the project's root directory
    void root_dir(const enumerate_path & a_path);
    //Version of target name for variables (generally
    //all uppercase version of target.)
    inline const QString & target_NAME() const {
          return target_NAME_;
        };
    //Set the version of target name to value created by to_target_NAME
    inline void target_NAME(QString pack, QString target) {
          target_NAME_ = this->build_target_NAME( pack, target );
        };
    static QString build_target_NAME(QString package_name, QString target_name);
    explicit base_generator();
    virtual ~base_generator();
    // Add necessary quoting to an input string.
    //
    // A user of the system may want to set a preprocessor 
    // flag of [-DSOMETEXT="Help Text."] as a property value
    // with the same text. The generators must be able to 
    // quote/escape this text so that it is correctly passed
    // to the final command.
    static QByteArray quote_string(QByteArray phrase);
};

} // namespace butter
#endif

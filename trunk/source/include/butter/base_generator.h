#ifndef BUTTER_BASE_GENERATOR_HPP
#define BUTTER_BASE_GENERATOR_HPP

/**
 * Base generator source file
 */
#include "butter/const_token_iterator.h"
#include <qstring.h>
#include "butter/utility.h"
#include <memory>

#include "butter/config.h"
namespace butter { class log; } 
namespace butter { class butter_constants; } 
class UmlArtifact;
class UmlPackage;
namespace butter { class location; } 
class UmlItem;
namespace butter { class compound_artifact; } 

namespace butter {

/**
 * The build generator derived classes are responsible for creating a set of build system artifacts
 * in a particular style.  These are inserted into the Uml Project where another class is 
 * responsible for writing them to the file-system.
 * 
 * The main action takes place in the create_system method.  The generator template class contains
 * a definitive implementation of this method that attempts to uniformly support all features of the
 * program, while calling derived class methods to implement the features for each style. The
 * Curiously Recurring Template Idiom is used to avoid virtual methods calls for the style specific 
 * functions.
 * 
 * Actions
 * * traverses a location tree examining packages
 * * creates a build-file artifact for all locations with targets
 * * links all the build-file artifacts together to form a build-system
 * 
 */
class base_generator
{
  public:
    /**
     * Indicate what sort of object is being installed.
     */
    enum install_type {
      bin = 0,/**
       * File is a binary file.
       */

      file = 1,/**
       * File is a generic file
       */

      lib = 2,/**
       * Item is a library
       */

      man = 3/**
       * File is a man page
       */


    };


  protected:
    /**
     * Enum of target types
     */
    enum target_type
    {
      executable = 0,/**
       * Target is an executable
       */

      static_library = 1,/**
       * Target is a static library
       */

      shared_library = 2,/**
       * Target is a shared library
       */

      other = 3/**
       * Target is an "other" library type.
       */


    };

    /**
     * Extra description for objects individually built from source
     */
    QString individual_obj_;

    /**
     * An "other" target library type name.  This is the name that is used for the
     * label/converted for the build rule.  See inidividual styles to see how this
     * is used and what requirements there are for supporting it in each style.
     */
    QString other_target_type_;


  private:
    /**
     * The root directory of the project.
     */
    pathcmp root_dir_;

    /**
     * The version of the target name to use with variables.
     */
    QString target_NAME_;


  public:
    /**
     * DRIVER METHOD
     * 
     * Create a series of build-system artifacts based at a_base location. The
     * artifacts are inserted into a package at each location.
     */
    virtual void create_system(location & a_base, const ::UmlItem & a_project) = 0;


  protected:
    /**
     * Parse a_artifact's properties and description (iff a_not_doc) looking for 
     * include directories  and linker options. Append what is found to a_hdr, 
     * a_link and a_flag.
     */
    static void find_hdr_link(const ::UmlItem & a_item, QString & a_hdr, QString & a_link, QString & a_flag, QString a_section_label, bool a_not_doc);

    /**
     * Find an artifact with a_name in a_loc. If not found then create and
     * initialise the document.
     * 
     * !!Warning!! returns nul iff a_loc.packages.empty
     */
    static std::auto_ptr< compound_artifact > get_artifact(location & a_loc, QString a_name);

    /**
     * The effect of this method is to convert the contents of a_list and a_addition
     * into lists of tokens.  These lists are merged, removing duplicate items, and
     * the new list is assigned back to a_list. Either list may be empty.
     */
    static void merge_string_list(QString & a_list, QString a_addition);

    /**
     * This method finds the substring in a_desc between "${"a_section"}"
     * and the next "${".  If no "${" is found then nothing is returned. Used
     * to get style specific data from target artifact description fields.
     */
    static QString section(QString a_section, QString a_desc);


  public:
    /**
     * Replace every occurence of find_char with rep_char in a_str
     */
    static void find_replace(QString & a_str, char find_char, char rep_char)
    {
      for (unsigned int i(0); i < a_str.length(); ++i)
      { a_str.constref(i) == find_char ? a_str.ref(i) = rep_char : QChar (find_char); }
    }

    /**
     * Get the projects root directory (if set)
     */
    const pathcmp &root_dir() const
    {
      return root_dir_;
    }

    /**
     * Set the project's root directory
     */
    void root_dir(const pathcmp & a_path)
    {
      root_dir_ = a_path;
    }

    /**
     * Version of target name for variables (generally
     * all uppercase version of target.)
     */
    const QString& target_NAME() const
    {
      return target_NAME_;
    }

    /**
     * Set the version of target name to value created by to_target_NAME
     */
    void target_NAME(const ::UmlArtifact & a_target)
    {
      target_NAME_ = to_target_NAME (a_target);
    }
    /**
     * Create the version of target name for variables (generally
     * all uppercase version of location + target.)
     */
    static QString to_target_NAME(const ::UmlArtifact & a_target);

};


} // namespace butter
#endif

#ifndef BUTTER_CMAKE_GENERATOR_HPP
#define BUTTER_CMAKE_GENERATOR_HPP


#include "butter/generator.h"
#include "butter/basic_style.h"
#include <qstring.h>
#include <memory>
#include "butter/base_generator.h"
#include <qtextstream.h>


namespace butter { class compound_document; } 
namespace butter { class compound_artifact; } 
class UmlArtifact;
namespace butter { class location; } 
class UmlItem;

namespace butter {

/**
 * Build file generator for cmake
 * 
 * Assumes external library xxx will set variables:
 * XXX_INCLUDE_DIRS
 * XXX_COMPILE_FLAGS
 * XXX_LINK_LIBRARIES
 * 
 * TODO:
 * need to root relative files with CMAKE_SOURCE_DIR
 * need to convert $(..) to ${..}
 * need to convert $(OUTPUTDIR) to CMAKE_BINARY_DIR
 * 
 * Additionally external library xxx without a local buildfile can be
 * found using:
 * find_package(Xxx)
 * 
 * \cond xdoc
 * The CMake style does not recognised round brackets, "(" and ")", as
 * variable delimiters.  The style generator will replace thes with "{" and "}"
 * in the generated files.
 * 
 * CMake automatically prepends "lib" to library targets on some platforms.
 * This plugout turns off this automatic behaviour for library targets.
 * \endcond xdoc
 */
class cmake_generator : public generator<cmake_generator> {
// Make out parent a friend.
friend class generator<cmake_generator>;
  public:
    static const basic_style style;

    /**
     * The default leaf filename for the current style
     */
    static const QString build_file_name;

    /**
     * The default leaf filename for the current style
     */
    static const QString& build_file_sysname;

    /**
     * Identify the minimum cmake version this generator supports.
     */
    static const QString cmake_minimum_required_;

    /**
     * This is the default contents of a the rules file (M_sys.mak) If a 
     * document artifact with name 'M_sys.mak' is not present when 
     * butter is executed with \@style='cmake' then one will be created
     * using this string.
     */
    static const char * default_rules[];

    /**
     * The name of the rules file.
     */
    static const QString rules_name;


  private:
    /**
     * Set of included libraries
     */
    QString lib_set_;

    /**
     * The top-level package name
     */
    QString project_name_;

    QString depend_set_;

    /**
     * When type is library, is this a static library?
     */
    bool is_static_type_;

    /**
     * List of languages to include (excluding C++)
     */
    QStringList language_set_;

    compound_artifact* sys_buildfile_;

    /**
     * Create a generator from the top-level a_project
     * 
     * \pre a_project.parent = nul
     */
    cmake_generator();

    /**
     * no copy
     */
    cmake_generator(const cmake_generator &) = delete;

    /**
     * no copy
     */
    cmake_generator(cmake_generator && source) = delete;

    /**
     * no assign
     */
    cmake_generator & operator=(const cmake_generator &) = delete;

  public:
    ~cmake_generator() throw () {}

    /**
     * Create bjam generator object.s
     */
    static std::unique_ptr< base_generator > create();


  private:
    /**
     * ** This method a library association to the current target entry for a_target.
     * 
     * Responsibilites
     * - Properties
     *  - associated includes, ldflags, cflags
     */
    void assoc_library(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString & a_includes, QString & a_ldflags, QString & a_cflags);

    /**
     * ** This method sets up object for creating a new target entry for a_target.
     * 
     * Responsibilites
     * - Properties
     *  - doc/source: includes, ldflags, cflags
     *  - compiler
     * - Other
     *  - set compilation for individual 
     */
    void assoc_source(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_filename, QString a_basename, QString a_src_inc, QString a_src_flags, bool a_isdoc);

    /**
     * Write the Jamfile descent links for a_location to a_os
     */
    void descendent_link(compound_artifact & a_art, compound_artifact & a_sys, const location & a_loc);

    /**
     * ** This method finalises the target entry for a_target.
     * 
     * NOTE: "Other" target requires WORKING_DIRECTORY to allow 
     * finding of source files. Alternatively, this could be
     * avoided by prepending ${CMAKE_CURRENT_SOURCE_DIR} to each 
     * source file. Currently WORKING_DIRECTORY is chosen as
     * it was felt that it is more obvious to the user and less
     * verbose.
     */
    void end_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_include, QString a_ldflags, QString a_cflags, QString a_compiler, base_generator::target_type a_type);

    /**
     * In this style external targets are not used, external library 
     * data is written in local-targets directly where it is 
     * referenced.
     */
    void external_target(const location & a_current, const ::UmlArtifact & a_target, compound_artifact & a_sys) {}

    /**
     * Write extra details to the top-level build file. 
     */
    void initialise(location & a_base, const ::UmlItem & a_project, compound_artifact & a_sys);

    /**
     * ** Create an install target.
     * 
     * Responsibilites
     * - Properties
     *  - install (library, executable and document)
     */
    void install_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_loc_var, base_generator::install_type a_type, bool a_isdoc);


  public:
    /**
     * Convert input string into CMake allowed string.  This
     * means converting () to {} and OUTPUTDIR to 
     * CMAKE_BINARY_DIR
     */
    QString mangle(QString input);


  private:
    /**
     * ** This method sets up object for creating a new target entry for a_target.
     * 
     * Responsibility
     * - Properties
     *  - compiler
     *  - include, ldflags, cflags
     *  - buildfile
     *  - type (library)
     * - Other
     *  - shared lib
     *  - static lib
     *  - executable
     *  - non-standard target
     */
    void start_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_build_file, QString a_compiler, base_generator::target_type a_type);

};


} // namespace butter
#endif

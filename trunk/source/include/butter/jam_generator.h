#ifndef BUTTER_JAM_GENERATOR_HPP
#define BUTTER_JAM_GENERATOR_HPP

/**
 * Source file for Jam generator
 */
#include "butter/config.h"
#include "butter/generator.h"
#include <qstring.h>
#include <qtextstream.h>
#include <memory>
#include "butter/base_generator.h"

namespace butter { class butter_constants; } 
class UmlArtifact;
namespace butter { class compound_artifact; } 
namespace butter { class location; } 
class UmlItem;

namespace butter {

/**
 * Build file generator for standard jam.
 * 
 * This is the build system generator of choice as is supports the widest range of
 * build options.
 * 
 * \cond XDOC
 * <style style="make">
 * <limitation>The <code>standard</code> jam tool does not provide
 * a shared library target. Providing a shared library for this style is on 
 * the <link target="todo">TODO list</link>.</limitation>
 * \endcond XDOC
 */
class jam_generator : public generator<jam_generator> {
// Make our parent a friend.
friend class generator<jam_generator>;
  public:
    static const QString build_file_name;

    static const QString build_file_sysname;

    static const QString comment_string;

    static const QString end_phrase;

    static const QString start_phrase;

    /**
     * The label for description 'sections' and the value of 
     * the style for this buildfile type.
     */
    static const QString section_name;


  private:
    /**
     * A template Jamrules.
     */
    static const char * default_rules[];

    /**
     * The name of the jam rules document.
     */
    static const QString rules_name;

    /**
     * Text for the a main targets associated sub-targets
     */
    QString lib_defn_;

    /**
     * Jam "Grist" of the current target location. Set in initialisation.
     */
    QString grist_;

    /**
     * The full target name for the currently processing target (defined in start_target)
     */
    QString qualified_target_name_;

    /**
     * ** This method a library association to the current target entry for a_target.
     * 
     * Responsibilites
     * - Properties
     *  - associated includes, ldflags, cflags
     */
    void assoc_library(const ::UmlArtifact & a_target, QTextStream & a_os, QString & a_includes, QString & a_ldflags, QString & a_cflags);

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
    void assoc_source(const ::UmlArtifact & a_target, QTextStream & a_os, QString a_filename, QString a_basename, QString a_src_inc, QString a_src_flags, bool a_isdoc);


  public:
    /**
     * Check a_source for butter properties and add information to a_os.
     * a_is_source is true of a_source stereotype is "source" and false
     * if stereotype is "document".
     */
    void check_properties(bool a_is_source, const ::UmlArtifact & a_source, QTextStream & a_os);

    /**
     * Create bjam generator object.s
     */
    static std::auto_ptr< base_generator > create();


  private:
    /**
     * Build the inter-buildfile link from this artifact/location to its parent and 
     * vice-versa. This is called just before the artifact is closed so has access 
     * to the (almost) complete content.
     */
    void descendent_link(compound_artifact & a_art, compound_artifact & a_sys, const location & a_loc);

    /**
     * ** This method finalises the target entry for a_target.
     */
    void end_target(const ::UmlArtifact & a_target, QTextStream & a_os, QString a_include, QString a_ldflags, QString a_cflags, QString a_compiler, base_generator::target_type a_type);

    /**
     * In this style external targets are not used, external library 
     * data is written in local-targets directly where it is 
     * referenced.
     * 
     * Responsibility
     * - Properties
     *  - project (defines external)
     *  - buildfile
     * - Other
     *  - external target
     */
    void external_target(const location & a_current, const ::UmlArtifact & a_target, compound_artifact & a_sys) {}

    /**
     * Write extra details to the top-level build file.
     * 
     * Responsibilities:
     * - Property handling
     *  - build-dir
     *  - project: flags, include, ldflags, (library) type
     *  - style
     *  - version
     */
    void initialise(location & a_base, const ::UmlItem & a_project, compound_artifact & a_sys);

    /**
     * ** Create an install target.
     * 
     * Responsibilites
     * - Properties
     *  - install (library, executable and document)
     */
    void install_target(const ::UmlArtifact & a_target, QTextStream & a_os, QString a_loc_var, base_generator::install_type a_type, bool a_isdoc);

    /**
     * Main ctor, takes top-level a_project.
     * 
     * \pre a_project.parent = nul
     */
    jam_generator();


  public:
    ~jam_generator() throw () {}


  private:
    /**
     * no copy
     */
    jam_generator(const jam_generator &);

    /**
     * no assign
     */
    jam_generator & operator=(const jam_generator &);
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
    void start_target(const ::UmlArtifact & a_target, QTextStream & a_os, QString a_build_file, QString a_compiler, base_generator::target_type a_type);

};


} // namespace butter
#endif

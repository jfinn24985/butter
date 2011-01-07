#ifndef BUTTER_BJAM_GENERATOR_HPP
#define BUTTER_BJAM_GENERATOR_HPP

/**
 * Boost Jam generator source file.
 */
#include "butter/generator.h"
#include <qstring.h>
#include <qtextstream.h>

#include <memory>
#include "butter/base_generator.h"

namespace butter { struct basic_style; } 
class UmlArtifact;
namespace butter { class compound_artifact; } 
namespace butter { class location; } 
class UmlItem;

namespace butter {

/**
 * Build file generator for boost-jam.
 * 
 * This is the build system of choice for C++-only projects that use
 * the Boost library.
 */
class bjam_generator : public generator<bjam_generator> {
// Make out parent a friend.
friend class generator<bjam_generator>;
  public:
    /**
     * The default leaf filename for the current style
     */
    static const QString build_file_name;

    /**
     * The default leaf filename for the current style
     */
    static const QString build_file_sysname;


  private:
    /**
     * The name of the project rules file.
     */
    static const QString rules_name;

    /**
     * This is the default contents of a the rules file (local.jam) If a 
     * document artifact with name 'local.jam' is not present when 
     * butter is executed with \@style='boost' then one will be created
     * using this string.
     */
    static const char * default_rules[];

    /**
     * Is this an alternate/other library target?
     */
    bool is_other_;

    /**
     * This name of the project
     */
    QString project_name_;

    /**
     * This method scans a_target for its direct sources and
     * direct and indirect dependencies. 
     */
    void assoc_library(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString & a_includes, QString & a_ldflags, QString & a_cflags);

    /**
     * This method scans a_target for its direct sources and
     * direct and indirect dependencies. 
     */
    void assoc_source(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_filename, QString a_basename, QString a_src_inc, QString a_src_flags, bool a_isdoc);

    bjam_generator()
    : is_other_ ()
    , project_name_ ()
    {}


  public:
    ~bjam_generator() throw () {}


  private:
    /**
     * no copy
     */
    bjam_generator(const bjam_generator &);


  public:
    /**
     * Create bjam generator object.s
     */
    static std::auto_ptr< base_generator > create();


  private:
    /**
     * Write the Jamfile descent links for a_location.
     */
    void descendent_link(compound_artifact & a_art, compound_artifact & a_sys, const location & a_loc);

    /**
     * This method scans a_target for its direct sources and
     * direct and indirect dependencies. 
     */
    void end_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_include, QString a_ldflags, QString a_cflags, QString a_compiler, base_generator::target_type a_type);

    /**
     * Write the external a_target at a_location. 
     * 
     * * searches system_artifact_ for an existing target
     * * new target sections are add to system_artifact_
     */
    void external_target(const location & a_current, const ::UmlArtifact & a_target, compound_artifact & a_sys);

    /**
     * Write extra details to the top-level build file.
     * 
     */
    void initialise(location & a_base, const ::UmlItem & a_project, compound_artifact & a_sys);

    /**
     * Create an install target.
     */
    void install_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_loc_var, base_generator::install_type a_type, bool a_isdoc);

    /**
     * no assign
     */
    bjam_generator & operator=(const bjam_generator &);
    /**
     * This method scans a_target for its direct sources and
     * direct and indirect dependencies. 
     */
    void start_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_build_file, QString a_compiler, base_generator::target_type a_type);


  public:
    /**
     * Process a set of flags as needed by boost. The general
     * process is to split a_flagset into space spearated tokens
     * and output each token with <a_flagname>"token". If a_combine 
     * is true then tokens without "-" or "/" as the first character
     * will be assumed to be arguments to the previous flag.
     */
    void process_flags(const ::QTextOStream & a_os, QString a_flagname, QString a_flagset, bool a_combine);

};


} // namespace butter
#endif

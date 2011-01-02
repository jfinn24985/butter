#ifndef BUTTER_BJAM_WRITER_HPP
#define BUTTER_BJAM_WRITER_HPP

/**
 * Boost Jam writer source file.
 */
#include "butter/config.h"
#include "butter/writer.h"
#include <qstring.h>
#include <qtextstream.h>
#include <memory>
#include "butter/build_writer.h"

namespace butter { class item_traits; } 
class UmlArtifact;
namespace butter { class compound_artifact; } 
namespace butter { class location; } 
class UmlItem;

namespace butter {

/**
 * Build file writer for boost-jam.
 * 
 * This is the build system of choice for C++-only projects that use
 * the Boost library.
 */
class bjam_writer : public writer<bjam_writer> {
// Make out parent a friend.
friend class writer<bjam_writer>;
  public:
    /**
     * The default leaf filename for the current style
     */
    static const QString build_file_name;

    /**
     * The default leaf filename for the current style
     */
    static const QString build_file_sysname;

    /**
     * Comment line prefix
     */
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
    void assoc_library(const ::UmlArtifact & a_target, QTextStream & a_os, QString & a_includes, QString & a_ldflags, QString & a_cflags);

    /**
     * This method scans a_target for its direct sources and
     * direct and indirect dependencies. 
     */
    void assoc_source(const ::UmlArtifact & a_target, QTextStream & a_os, QString a_filename, QString a_basename, QString a_src_inc, QString a_src_flags, bool a_isdoc);

    bjam_writer()
    : is_other_ ()
    , project_name_ ()
    {}


  public:
    ~bjam_writer() throw () {}


  private:
    /**
     * no copy
     */
    bjam_writer(const bjam_writer &);


  public:
    /**
     * Create bjam writer object.s
     */
    static std::auto_ptr< build_writer > create();


  private:
    /**
     * Write the Jamfile descent links for a_location.
     */
    void descendent_link(compound_artifact & a_art, compound_artifact & a_sys, const location & a_loc);

    /**
     * This method scans a_target for its direct sources and
     * direct and indirect dependencies. 
     */
    void end_target(const ::UmlArtifact & a_target, QTextStream & a_os, QString a_include, QString a_ldflags, QString a_cflags, QString a_compiler, build_writer::target_type a_type);

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
    void install_target(const ::UmlArtifact & a_target, QTextStream & a_os, QString a_loc_var, build_writer::install_type a_type, bool a_isdoc);

    /**
     * no assign
     */
    bjam_writer & operator=(const bjam_writer &);
    /**
     * This method scans a_target for its direct sources and
     * direct and indirect dependencies. 
     */
    void start_target(const ::UmlArtifact & a_target, QTextStream & a_os, QString a_build_file, QString a_compiler, build_writer::target_type a_type);

};


} // namespace butter
#endif

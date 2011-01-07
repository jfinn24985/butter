#ifndef BUTTER_GMAKE_GENERATOR_HPP
#define BUTTER_GMAKE_GENERATOR_HPP

/**
 * Source for GNU make generator
 */
#include "butter/generator.h"
#include <qstring.h>
#include <qtextstream.h>

#include <memory>
#include "butter/base_generator.h"

class UmlArtifact;
namespace butter { class compound_artifact; } 
namespace butter { class location; } 
class UmlItem;

namespace butter {

/**
 * Build file generator for gnu make
 * 
 * Requires GNU make compatible make, for generic make use the CMake %style
 * \ref cmake_generator .
 * 
 * \cond xdoc
 * <xdoc:section part-of="limitation/make" ><p>The <code>make</code> program
 * can not create target dependency graph between directories.  As all the other
 * styles do create such a graph, this plugout does not. This can require calling
 * make several times to get all targets to link correctly.</p>
 * <xdoc:subsection name="workaround" ><p>The initially generated makefile will include targets and
 * subdirectories in the order they appear in the project.  Once created, the
 * order of target sections in the generated artifact is maintained so manually
 * reordering the target sections within a single artifact will change their compilation
 * order.</p></xdoc:subsection>
 * <xdoc:title>Limitations of the Make style build system</xdoc:title></xdoc:section>
 * <xdoc:section part-of="extension/build-environ/make" >
 * <xdoc:title>Extending Make style to new compilers and operating
 * environments</xdoc:title>
 * <p>New compiler and operating systems can be added by creating system makefiles
 * like M_gcc.mak and M_unix.mak.  See <xdoc:xref to="customisation/build-environ" >
 * customisation section</xdoc:xref> for how to include new system files.
 * To be portable you should only specify tools
 * that would be available with the standard installations of the new compiler suite or
 * operating system. Listed below are the minimal
 * set of variables directly used by this style and so must be defined.</p>
 * <p>The build system uses the following variables to control the build
 * process. The first two select the makefiles with the compiler and operating
 * system specific data.  Note the operating system file is always included first,
 * so it can not rely on variables that will be set in the compiler file. Alternatively
 * the compiler file can <em>override</em> definitions if required. The
 * last variable selects the optimisation level
 * to use.  These variables must be set on the command line or as environment
 * variables or at the top of the first makefile.</p>
 * <dl><dt>CC</dt><dd>C compiler</dd>
 * <dt>OS</dt><dd>The operating system type, eg unix</dd>
 * <dt>VARIANT</dt><dd>The build system expects optimisation flags
 * to be added according to the value of this variable. A value of
 * <code>RELEASE</code> should add standard optimisation flags to the relevant
 * preprocessor and language flags.  A value of <code>DEBUG</code> or undefined or
 * an unrecognised value should add debugging flags. Other actions are
 * allowed, such as using a dummy install location when set to DEBUG.</dd></dl>
 * <p>The build system requires the following variables to be set for
 * each compiler suite.  You may also need to set other variables
 * that GNU make uses, such as <code>AR</code>, <code>RM</code> and <code>LINK</code>, for the new
 * build environment.</p>
 * <dl><dt>CCC</dt><dd>C++ compiler</dd>
 * <dt>CPPFLAGS</dt><dd>Preprocessor flags</dd>
 * <dt>CCFLAGS</dt><dd>C language flags</dd>
 * <dt>CCCFLAGS</dt><dd>C++ language flags</dd>
 * <dt>LDFLAGS</dt><dd>Linker flags</dd></dl>
 * Commonly defined optional variables are.
 * <dl><dt>FORTRAN</dt><dd>Fortran compiler </dd>
 * <dt>FORTRANFLAGS</dt><dd>Fortran language flags</dd>
 * <dt>OPENMP</dt><dd>OpenMP compiler switch</dd></dl>
 * <p>The following macros and definitions must be defined
 * in either the operating system or compiler definition makefiles.  For how
 * to define a makefile macro refer to the GNU make documentation.</p>
 * <dl><dt>do_link_exe</dt><dd>Creates an executable. Takes two
 * arguments: the program name and a list of objects.</dd>
 * <dt>do_link_shr</dt><dd>Creates a shared library. Takes three
 * arguments: the library name, a list of objects and an optional
 * version number. The prefix <code>lib</code>, used on many unices,
 * must <em>not</em> be automatically added.</dd>
 * <dt>do_archive</dt><dd>Creates a static library. Takes two
 * arguments: the library name and a list of objects. The
 * prefix <code>lib</code>, used on many unices,
 * must <em>not</em> be automatically added.</dd>
 * <dt>do_install</dt><dd>Installs a target.  Takes three arguments: the target
 * name, the destination <em>identifier</em> and permission <em>identifier</em>.
 * These identifiers are prepended to <code>DIR</code> to form the variable name
 * containing the destination directory and to <code>IFLAGS</code> for permissions.
 * The third argument is optional if it would be the same as the second. For
 * example a the identifier <code>BIN</code> would select the
 * <code>$(BINDIR)</code> directory variable or the the <code>$(BINIFLAGS)</code>
 * permission variable. The minimum set of directory and permission flags required
 * by the make style are:<br />
 * <dl><dt>BINDIR, BINIFLAGS</dt><dd>For executable targets</dd>
 * <dt>LIBDIR, LIBIFLAGS</dt><dd>For shared library targets</dd>
 * <dt>DOCDIR</dt><dd>For documentation targets</dd>
 * <dt>MANDIR, MANIFLAGS</dt><dd>For man-page or help targets</dd>
 * <dt>FILEIFLAGS</dt><dd>For general file targets</dd>
 * </dl></dd>
 * <dt>doit</dt><dd>Macro to recursively descend source tree.
 * Takes two arguments, the current make target (eg "clean")
 * and a list of directories. It should call <code>$(MAKE) -C [subdir] [target]</code>
 * for each directory in the list.</dd>
 * <dt>%$(sufdep): %.[???]</dt><dd>A set of implicit rules for
 * creating a dependency file for each source file extension. This
 * can be a dummy operation that creates an empty file if the
 * dependency information is not required.</dd></dl>
 * <p>The following variables for operating system settings
 * are expected.</p>
 * <dl><dt>sufobj</dt><dd>File suffix of object files</dd>
 * <dt>sufexe</dt><dd>File suffix of program</dd>
 * <dt>suflib</dt><dd>File suffix of static libraries</dd>
 * <dt>sufshr</dt><dd>File suffix of shared librraries</dd>
 * <dt>sufdep</dt><dd>File suffix of generated dependency files</dd>
 * <dt>SLASH</dt><dd>File path separator</dd></dl>
 * </xdoc:section >
 * \endcond xdoc
 */
class gmake_generator : public generator<gmake_generator> {
// Make out parent a friend.
friend class generator<gmake_generator>;
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
     * This is the default contents of a the rules file (M_sys.mak).
     * If a document artifact with name 'M_sys.mak' is not present when 
     * %butter is executed with \@style='standard' then one will be created
     * using this string.
     */
    static const char * default_rules_sys[];

    /**
     * This is the default contents of the rules file (M_cl.mak) for
     * the microsoft compiler. If a 
     * document artifact with name 'M_cl.mak' is not present when 
     * %butter is executed with \@style='standard' then one will be created
     * using this string.
     * 
     */
    static const char * default_rules_cl[];

    /**
     * This is the default contents of the rules file (M_gcc.mak) for
     * the GNU compiler collection. If a 
     * document artifact with name 'M_gcc.mak' is not present when 
     * %butter is executed with \@style='standard' then one will be created
     * using this string.
     */
    static const char * default_rules_gcc[];

    /**
     * This is the default contents of the rules file (M_unix.mak) for
     * a UNiX/POSIX like operating system. If a 
     * document artifact with name 'M_unix.mak' is not present when 
     * %butter is executed with \@style='standard' then one will be created
     * using this string.
     * 
     */
    static const char * default_rules_unix[];

    /**
     * This is the default contents of the rules file (M_Windows_NT.mak) for
     * a (post NT) Micrsoft Windows like operating system. If a 
     * document artifact with name 'M_Windows_NT.mak' is not present when 
     * %butter is executed with \@style='standard' then one will be created
     * using this string.
     */
    static const char * default_rules_winnt[];

    /**
     * Null terminated list of the default_rules_[] attributes.
     * This is a simple list of the default_rules_[...] attributes, listed
     * in the same order as the rules_name attribute. It is only
     * used to initialise the default_rules attribute. 
     */
    static const char ** default_rules_list[];

    /**
     * A list containing _all_ the default rules.
     * This is assigned all the default rules concatenated together. The
     * rules appear in the order specified by rules_name.
     * \sa default_rules_list
     */
    static const char ** default_rules;

    /**
     * A space separated list of rule names.
     * This style has multiple system buildfiles. This variable contains a space
     * separated list of the names of these files.
     */
    static const QString rules_name;

    /**
     * Set of included libraries
     */
    QString lib_set_;

    /**
     * The target name as used by make
     */
    QString qualified_target_name_;

    /**
     * ** This method a library association to the current target entry for a_target.
     * 
     * Responsibilites
     * - Properties
     *  - associated includes, ldflags, cflags
     */
    void assoc_library(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString & a_includes, QString & a_ldflags, QString & a_cflags);

    /**
     *  This method sets up object for creating a new target entry for a_target.
     * 
     * Responsibilites
     * - Properties
     *  - doc/source: includes, ldflags, cflags
     *  - compiler
     * - Other
     *  - set compilation for individual 
     * 
     * \cond XDOC
     * <property style="standard" name="include" on="artifact" subtype="stereotype=source"> Add
     * include directories to compilation of this source only.  Any include directories
     * needed to compile dependents of this source should be set in this property on
     * the target artifact that is associated to the source.</property>
     * \endcond XDOC
     */
    void assoc_source(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_filename, QString a_basename, QString a_src_inc, QString a_src_flags, bool a_isdoc);


  public:
    /**
     * Create bjam generator object.s
     */
    static std::auto_ptr< base_generator > create();


  private:
    /**
     * Write the Jamfile descent links for a_location to a_os
     */
    void descendent_link(compound_artifact & a_art, compound_artifact & a_sys, const location & a_loc);

    /**
     * ** This method finalises the target entry for a_target.
     */
    void end_target(const ::UmlArtifact & a_target, ::QTextOStream & a_os, QString a_include, QString a_ldflags, QString a_cflags, QString a_compiler, base_generator::target_type a_type);

    /**
     * In this style external targets are not used, external library 
     * data is written in local-targets directly where it is 
     * referenced.
     */
    void external_target(const location & a_current, const ::UmlArtifact & a_target, compound_artifact & a_sys) {}

    /**
     * Create a generator from the top-level a_project
     * 
     * \pre a_project.parent = nul
     */
    gmake_generator();


  public:
    ~gmake_generator() throw () {}


  private:
    /**
     * no copy
     */
    gmake_generator(const gmake_generator &);

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

    /**
     * no assign
     */
    gmake_generator & operator=(const gmake_generator &);
    /**
     * Process a_inc_list and a_flag_list into preprocessor
     * and compiler flag sets. Also checks if paths are relative
     * in which case it sets them relative to root-dir
     */
    void process_flags(QString a_inc_list, QString a_flag_list, QString & a_cppflags, QString & a_cflags, QString & a_ldflags);

    /**
     * Transform a space separated list of include dirs into a
     * list with -I...
     */
    static QString process_hdrs(QString a_inc_list);

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

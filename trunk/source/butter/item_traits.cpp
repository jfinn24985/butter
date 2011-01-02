
#include "butter/item_traits.h"

#include "version.h"
namespace butter {

/**
 * Then label of the install target binary directory.
 */
const QString item_traits::bindir_value("BINDIR");

/**
 * Artifacts with the document stereotype that contain this
 * string are considered to be templates
 */
const QString item_traits::buildfile_value("buildfile");

/**
 * The current program version string
 */
const QString item_traits::BUTTER_VERSION = "Butter version 1.2beta." BUTTER_SVN_VERSION_;

/**
 * User property name:
 * * indicates the relationship between the package and the root directory set in C++ settings.
 * * only used if present on the top-level project
 */
const QString item_traits::butter_base_name("butter base");

/**
 * User property name:
 * * indicates where the project should be compiled.  This is not supported
 * on all build platforms.
 * * only useful on the project package.
 */
const QString item_traits::butter_build_dir_name("butter build-dir");

/**
 * User property name:
 * * indicates the name of a buildfile to include for the current library.
 * * used with 'library' artifacts which also have butter_project_name property to indicate
 * they are external projects.
 */
const QString item_traits::butter_buildfile_name("butter buildfile");

/**
 * User property name:
 * * indicates the name of a compiler tag/variable to use with this artifact.
 */
const QString item_traits::butter_compiler_name("butter compiler");

/**
 * User property name:
 * * indicates c++flags to use for the current target artifact. This property is automatically 
 * inherited from parent objects if not overriden
 * * only used on target artifacts (library or executable artifacts)
 */
const QString item_traits::butter_flags_name("butter flags");

/**
 * The name of the install property
 */
const QString item_traits::butter_install_name("butter install");

/**
 * User property name:
 * * indicates ldflags to use for the current target artifact (\see link_label). This property is 
 * automatically inherited from parent objects if not overriden
 * * only used on target artifacts (library or executable artifacts)
 */
const QString item_traits::butter_ldflags_name("butter ldflags");

/**
 * User property name:
 * * indicates whether the library artifact is static or dynamic. will have value 
 * shared_value or static_value. This property is automatically inherited from 
 * parent objects if not overriden.
 * * only used on library target artifacts.
 */
const QString item_traits::butter_lib_type_name("butter type");

/**
 * User property name:
 * * indicates how much information to print:
 * 0 = (default) warning and error messages
 * 1 = as for 0 and information messages
 * 2 = as for 1 and debugging information
 */
const QString item_traits::butter_log_name("butter log-level");

/**
 * User property name
 * * Indicates that a library artifact should be built as another target type. 
 * For jam based builds this is used directly as the target-type name, for make
 * based builds the buidl rule will replace "$(CCC)" with "$(VALUE)" (in 
 * uppercase)
 * * For document artifacts if the property value is 'nowrite' then the document is not written
 * to the filesystem.
 * * This property only used on library artifact targets.
 */
const QString item_traits::butter_other_name("butter other");

/**
 * User property name:
 * * indicates extra include directories for the current target artifact. The value should be 
 * a a list of directory names (\see header_label)
 * * only used on target artifacts (library or executable artifacts)
 */
const QString item_traits::butter_include_name("butter include");

/**
 * User property name:
 * * indicates the artifact represents an object from an external project. The property value is the name of the external project.
 * * only used if present on artifacts with the library stereotype
 * 
 * \see butter_buildfile_name
 */
const QString item_traits::butter_project_name("butter project");

/**
 * User property name:
 * * indicates the style of build system/files to create, defaults to 'boost'
 * * only used if present on the top-level project
 */
const QString item_traits::butter_style_name("butter style");

/**
 * Property name for the project version.
 */
const QString item_traits::butter_version_name("butter version");

/**
 * The stereotype name for document artifacts.
 */
const QString item_traits::document_stereotype("document");

/**
 * The stereotype name for executable artifacts.
 */
const QString item_traits::executable_stereotype("executable");

/**
 * The  label of a line of std flags in a 'generic' description section. 
 * eg:
 * 
 * FLAGS = -DHAVE_ALLOCA
 */
const QString item_traits::flag_label("FLAGS");

/**
 * The label of a 'generic' section of an artifact description.
 */
const QString item_traits::generic_section("generic");

/**
 * The label of a line of include directories in a 'generic' description section. This
 * list should contain directorie names without the "-I" include flag. The "-I' will be
 * added when necessary. eg:
 * 
 * HDR=/dir1 /dir2
 */
const QString item_traits::header_label("HDR");

/**
 * The label of the install target library directory var.
 */
const QString item_traits::libdir_value("LIBDIR");

/**
 * The stereotype name for library artifacts.
 */
const QString item_traits::library_stereotype("library");

/**
 * The  label of a line of link flags in a 'generic' description section. 
 * eg:
 * 
 * LINK = -L/my/path -lmylib
 */
const QString item_traits::link_label("LINK");

/**
 * Value of he "butter install" property to indicate that the target 
 * should not be installed.
 */
const QString item_traits::no_install_value("NONE");

/**
 * The prefix for the beginning of section labels that
 * might be found in artifact descriptions.
 */
const QString item_traits::section_prefix("butter_");

/**
 * Property value for "butter_lib_type" property. Indicates that
 * library targets will be shared.
 */
const QString item_traits::shared_value("shared");

/**
 * The stereotype name for source artifacts.
 */
const QString item_traits::source_stereotype("source");

/**
 * Property value for "butter_lib_type" property. Indicates that
 * library targets will be static.
 */
const QString item_traits::static_value("static");

/**
 * Generic property name for the project version.
 */
const QString item_traits::version_name("version");

/**
 * Build-file section name for the project version.
 */
const QString item_traits::version_label("version");

/**
 * Build file section name for the date.
 */
const QString item_traits::date_label("date");

/**
 * Build file section name for the preamble.
 */
const QString item_traits::preamble_label("preamble");

/**
 * Build file section name for the closing section.
 */
const QString item_traits::close_label("close");

/**
 * Build file section label for a target section.
 */
const QString item_traits::target_label("target");

/**
 * Build file section label for a target section.
 */
const QString item_traits::app_data_env_var = 
#ifdef _WIN32
"APPDATA"
#else
"HOME"
#endif
;

/**
 * Build file section label for a target section.
 */
const QString item_traits::app_data_subdir_name = 
#ifdef _WIN32
"Butter"
#else
".butter"
#endif
;


} // namespace butter

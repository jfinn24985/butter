
#include "constants.h"

#include <qtextstream.h>
namespace butter {

//The label of the install target binary directory.

const QString constants::bindir_value("BINDIR");

//Artifacts with the document stereotype that contain this
//string are considered to be templates

const QString constants::buildfile_value("buildfile");

//The current program version string

const QString constants::BUTTER_VERSION= "Butter version 0.2beta.";

//User property name:
//* indicates the relationship between the package and the root directory set in C++ settings.
//* only used if present on the top-level project

const QString constants::butter_base_name("butter base");

//User property name:
//* indicates where the project should be compiled.  This is not supported
//on all build platforms.
//* only useful on the project package.

const QString constants::butter_build_dir_name("butter build-dir");

//User property name:
//* indicates the name of a buildfile to include for the current library.
//* used with 'library' artifacts which also have butter_project_name property to indicate
//they are external projects.

const QString constants::butter_buildfile_name("butter buildfile");

//User property name:
//* indicates the name of a compiler tag/variable to use with this artifact.

const QString constants::butter_compiler_name("butter compiler");

//User property name:
//* indicates c++flags to use for the current target artifact. This property is automatically 
//inherited from parent objects if not overriden
//* only used on target artifacts (library or executable artifacts)

const QString constants::butter_flags_name("butter flags");

//The name of the install property

const QString constants::butter_install_name("butter install");

//User property name:
//* indicates ldflags to use for the current target artifact (\see link_label). This property is 
//automatically inherited from parent objects if not overriden
//* only used on target artifacts (library or executable artifacts)

const QString constants::butter_ldflags_name("butter ldflags");

//User property name:
//* indicates whether the library artifact is static or dynamic. will have value 
//shared_value or static_value. This property is automatically inherited from 
//parent objects if not overriden.
//* only used on library target artifacts.

const QString constants::butter_lib_type_name("butter type");

//User property name:
//* indicates how much information to print:
//0 = (default) warning and error messages
//1 = as for 0 and information messages
//2 = as for 1 and debugging information

const QString constants::butter_log_name("butter log-level");

//User property name
//* Indicates that a library artifact should be built as another target type. 
//For jam based builds this is used directly as the target-type name, for make
//based builds the build rule will replace "$(CCC)" with "$(VALUE)" (in 
//uppercase)
//* For document artifacts if the property value is 'nowrite' then the document is not written
//to the filesystem.
//* This property only used on library artifact targets.

const QString constants::butter_other_name("butter other");

//User property name:
//* indicates extra include directories for the current target artifact. The value should be 
//a a list of directory names (\see header_label)
//* only used on target artifacts (library or executable artifacts)

const QString constants::butter_include_name("butter include");

//User property name:
//* indicates the artifact represents an object from an external project. The property value is the name of the external project.
//* only used if present on artifacts with the library stereotype
//
//\see butter_buildfile_name

const QString constants::butter_project_name("butter project");

//User property name:
//* indicates the style of build system/files to create, defaults to 'boost'
//* only used if present on the top-level project

const QString constants::butter_style_name("butter style");

//Property name for the project version.

const QString constants::butter_version_name("butter version");

//The stereotype name for document artifacts.

const QString constants::document_stereotype("document");

//The stereotype name for executable artifacts.

const QString constants::executable_stereotype("executable");

//The  label of a line of std flags in a 'generic' description section. 
//eg:
//
//FLAGS = -DHAVE_ALLOCA

const QString constants::flag_label("FLAGS");

//The label of a 'generic' section of an artifact description.

const QString constants::generic_section("generic");

//The label of a line of include directories in a 'generic' description section. This
//list should contain directorie names without the "-I" include flag. The "-I' will be
//added when necessary. eg:
//
//HDR=/dir1 /dir2

const QString constants::header_label("HDR");

//The label of the install target library directory var.

const QString constants::libdir_value("LIBDIR");

//The stereotype name for library artifacts.

const QString constants::library_stereotype("library");

//The  label of a line of link flags in a 'generic' description section. 
//eg:
//
//LINK = -L/my/path -lmylib

const QString constants::link_label("LINK");

//Value of he "butter install" property to indicate that the target 
//should not be installed.

const QString constants::no_install_value("NONE");

//The prefix for the beginning of section labels that
//might be found in artifact descriptions.

const QString constants::section_prefix("butter_");

//Property value for "butter_lib_type" property. Indicates that
//library targets will be shared.

const QString constants::shared_value("shared");

//The stereotype name for source artifacts.

const QString constants::source_stereotype("source");

//Property value for "butter_lib_type" property. Indicates that
//library targets will be static.

const QString constants::static_value("static");

//Generic property name for the project version.

const QString constants::version_name("version");

//Build-file section name for the project version.

const QString constants::version_label("version");

//Build file section name for the date.

const QString constants::date_label("date");

//Build file section name for the preamble.

const QString constants::preamble_label("preamble");

//Build file section name for the closing section.

const QString constants::close_label("close");

//Build file section label for a target section.

const QString constants::target_label("target");

//Build file section label for a target section.

const QString constants::app_data_env_var= 
#ifdef _WIN32
"APPDATA"
#else
"HOME"
#endif
;

//Build file section label for a target section.

const QString constants::app_data_subdir_name= 
#ifdef _WIN32
"Butter"
#else
".butter"
#endif
;

//User property name for UMLArtifacts
//
//* For artifacts with stereotype "document" if the property value is 
//'nowrite' then the document is not written to the filesystem.  Any
//other value will be used as a filename extension when construting
//the artifact's path.
QByteArray constants::butter_output_label()
{
  return "butter output"; }

//Value of "butter_output" property that indicates an artifact
//should not be considered for writing to the file system.
QByteArray constants::nowrite_value()
{
  return "nowrite"; }

//Transcribe the given text into a comment in the current
//build system.  In general this prepends comment indicators
//to each line of a_text and adds a terminal newline if
//a_text does not end in a newline.
QString constants::comment_text(QString a_text)
{
QString Result;
{
  QTextOStream os_ (&Result);
  int s_ = 0; // start position
  for (int e_ = a_text.find ('\n', s_); e_ > 0; e_ = a_text.find ('\n', s_))
  {
    os_ << begin_comment << ' ';
    os_ << a_text.mid (s_, e_ - s_);
    os_ << end_comment << "\n";
    s_ = e_ + 1;
  }
  if (a_text.at (a_text.length () - 1) != '\n')
  {
    os_ << begin_comment << ' ';
    os_ << a_text.mid (s_);
    os_ << end_comment << "\n";
  }
}
return Result;

}

const QString constants::begin_comment{ "#" };

const QString constants::end_comment{};

const QString & constants::text_template_xmlname()
{
  static QString label("template"); return label; 
}

const QString & constants::label_xmlname()
{
  static QString label("label"); return label; 
}

const QString & constants::replacement_xmlname()
{
  static QString label("replace"); return label;
}

const QString & constants::optional_xmlname()
{
  static QString label("option"); return label;
}

const QString & constants::build_file_xmlname()
{
  static QString label("build-file"); return label;
}

const QString & constants::root_file_xmlname()
{
  static QString label("root-file"); return label;
}

const QString & constants::binary_target_xmllabel()
{
  static QString label("binary-target");
  return label;
}

const QString & constants::style_xmlname()
{
  static QString label("style");
  return label;
}

const QString & constants::file_xmlname()
{
  static QString label("file");
  return label;
}

const QString & constants::property_xmlname()
{
  static QString label("property");
  return label;
}

const QString & constants::close_xmllabel()
{
  static QString label("close");
  return label;
}

const QString & constants::end_phrase_xmlname()
{
  static QString label("end-phrase");
  return label;
}

const QString & constants::start_phrase_xmlname()
{
  static QString label("start-phrase");
  return label;
}

const QString & constants::preamble_xmllabel()
{
  static QString label("preamble");
  return label;
}

const QString & constants::version_xmllabel()
{
  static QString label("version");
  return label;
}

const QString & constants::date_xmllabel()
{
  static QString label("date");
  return label;
}


} // namespace butter

#ifndef BUTTER_CONSTANTS_H
#define BUTTER_CONSTANTS_H


#include <qstring.h>
#include <qbytearray.h>

namespace butter {

//Static strings for the butter project.
class constants {
  public:
    //The label of the install target binary directory.
    
    static const QString bindir_value;

    //Artifacts with the document stereotype that contain this
    //string are considered to be templates
    
    static const QString buildfile_value;

    //The current program version string
    
    static const QString BUTTER_VERSION;

    //User property name:
    //* indicates the relationship between the package and the root directory set in C++ settings.
    //* only used if present on the top-level project
    
    static const QString butter_base_name;

    //User property name:
    //* indicates where the project should be compiled.  This is not supported
    //on all build platforms.
    //* only useful on the project package.
    
    static const QString butter_build_dir_name;

    //User property name:
    //* indicates the name of a buildfile to include for the current library.
    //* used with 'library' artifacts which also have butter_project_name property to indicate
    //they are external projects.
    
    static const QString butter_buildfile_name;

    //User property name:
    //* indicates the name of a compiler tag/variable to use with this artifact.
    
    static const QString butter_compiler_name;

    //User property name:
    //* indicates c++flags to use for the current target artifact. This property is automatically 
    //inherited from parent objects if not overriden
    //* only used on target artifacts (library or executable artifacts)
    
    static const QString butter_flags_name;

    //The name of the install property
    
    static const QString butter_install_name;

    //User property name:
    //* indicates ldflags to use for the current target artifact (\see link_label). This property is 
    //automatically inherited from parent objects if not overriden
    //* only used on target artifacts (library or executable artifacts)
    
    static const QString butter_ldflags_name;

    //User property name:
    //* indicates whether the library artifact is static or dynamic. will have value 
    //shared_value or static_value. This property is automatically inherited from 
    //parent objects if not overriden.
    //* only used on library target artifacts.
    
    static const QString butter_lib_type_name;

    //User property name:
    //* indicates how much information to print:
    //0 = (default) warning and error messages
    //1 = as for 0 and information messages
    //2 = as for 1 and debugging information
    
    static const QString butter_log_name;

    //User property name
    //* Indicates that a library artifact should be built as another target type. 
    //For jam based builds this is used directly as the target-type name, for make
    //based builds the build rule will replace "$(CCC)" with "$(VALUE)" (in 
    //uppercase)
    //* For document artifacts if the property value is 'nowrite' then the document is not written
    //to the filesystem.
    //* This property only used on library artifact targets.
    
    static const QString butter_other_name;

    //User property name:
    //* indicates extra include directories for the current target artifact. The value should be 
    //a a list of directory names (\see header_label)
    //* only used on target artifacts (library or executable artifacts)
    
    static const QString butter_include_name;

    //User property name:
    //* indicates the artifact represents an object from an external project. The property value is the name of the external project.
    //* only used if present on artifacts with the library stereotype
    //
    //\see butter_buildfile_name
    
    static const QString butter_project_name;

    //User property name:
    //* indicates the style of build system/files to create, defaults to 'boost'
    //* only used if present on the top-level project
    
    static const QString butter_style_name;

    //Property name for the project version.
    
    static const QString butter_version_name;

    //The stereotype name for document artifacts.
    
    static const QString document_stereotype;

    //The stereotype name for executable artifacts.
    
    static const QString executable_stereotype;

    //The  label of a line of std flags in a 'generic' description section. 
    //eg:
    //
    //FLAGS = -DHAVE_ALLOCA
    
    static const QString flag_label;

    //The label of a 'generic' section of an artifact description.
    
    static const QString generic_section;

    //The label of a line of include directories in a 'generic' description section. This
    //list should contain directorie names without the "-I" include flag. The "-I' will be
    //added when necessary. eg:
    //
    //HDR=/dir1 /dir2
    
    static const QString header_label;

    //The label of the install target library directory var.
    
    static const QString libdir_value;

    //The stereotype name for library artifacts.
    
    static const QString library_stereotype;

    //The  label of a line of link flags in a 'generic' description section. 
    //eg:
    //
    //LINK = -L/my/path -lmylib
    
    static const QString link_label;

    //Value of he "butter install" property to indicate that the target 
    //should not be installed.
    
    static const QString no_install_value;

    //The prefix for the beginning of section labels that
    //might be found in artifact descriptions.
    
    static const QString section_prefix;

    //Property value for "butter_lib_type" property. Indicates that
    //library targets will be shared.
    
    static const QString shared_value;

    //The stereotype name for source artifacts.
    
    static const QString source_stereotype;

    //Property value for "butter_lib_type" property. Indicates that
    //library targets will be static.
    
    static const QString static_value;

    //Generic property name for the project version.
    
    static const QString version_name;

    //Build-file section name for the project version.
    
    static const QString version_label;

    //Build file section name for the date.
    
    static const QString date_label;

    //Build file section name for the preamble.
    
    static const QString preamble_label;

    //Build file section name for the closing section.
    
    static const QString close_label;

    //Build file section label for a target section.
    
    static const QString target_label;

    //Build file section label for a target section.
    
    static const QString app_data_env_var;

    //Build file section label for a target section.
    
    static const QString app_data_subdir_name;

    //User property name for UMLArtifacts
    //
    //* For artifacts with stereotype "document" if the property value is 
    //'nowrite' then the document is not written to the filesystem.  Any
    //other value will be used as a filename extension when construting
    //the artifact's path.
    static QByteArray butter_output_label();

    //Value of "butter_output" property that indicates an artifact
    //should not be considered for writing to the file system.
    static QByteArray nowrite_value();

    //Transcribe the given text into a comment in the current
    //build system.  In general this prepends comment indicators
    //to each line of a_text and adds a terminal newline if
    //a_text does not end in a newline.
    static QString comment_text(QString a_text);
    static const QString begin_comment;


  private:
    static const QString end_comment;


  public:
    static const QString & text_template_xmlname();

    static const QString & label_xmlname();

    static const QString & replacement_xmlname();

    static const QString & optional_xmlname();

    static const QString & build_file_xmlname();

    static const QString & root_file_xmlname();

    static const QString & binary_target_xmllabel();

    static const QString & style_xmlname();

    static const QString & file_xmlname();

    static const QString & property_xmlname();

    static const QString & close_xmllabel();

    static const QString & end_phrase_xmlname();

    static const QString & start_phrase_xmlname();

    static const QString & preamble_xmllabel();

    static const QString & version_xmllabel();

    static const QString & date_xmllabel();

};

} // namespace butter
#endif

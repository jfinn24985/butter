#ifndef BUTTER_BUILD_STYLE_H
#define BUTTER_BUILD_STYLE_H


#include <qstring.h>

#include <qmap.h>
namespace butter { class text_template; } 

class QXmlStreamReader;
namespace butter {

class build_style {
  public:
    typedef QMap< QString, QString > map_type;


  private:
    QMap<QString, text_template> templates_;

    map_type properties_;

    //Map of filenames and content for special/auxilliary build files to create in the root location.
    map_type special_files_;


  public:
    build_style();

    ~build_style();


  private:
    build_style(const build_style & source) = delete;

    build_style & operator=(const build_style & source) = delete;


  public:
    void read_style(QString style, QXmlStreamReader & xml);

    //Add the contents of other to the style property map, with any
    //existing keys being overwritten by the values in other.
    map_type extend_properties(const map_type & other) const;

    //Name of all non-root build files.
    QString get_build_file_name();

    //A map of filenames and content of any special build files to create in the root location
    const map_type& get_properties() const;

    //Name of the root build file.
    QString get_root_file_name();

    //A map of filenames and content of any special build files to create in the root location
    const map_type& get_special_files() const;

    //A map of filenames and content of any special build files to create in the root location
    QMap< QString, text_template > const& get_templates() const;

    //Adds/Checks the close section.
    //
    //* Using this method instead of directly accessing the templates is
    //recommended as it combines the passed properties to the build
    //style properties before instantiating the template.
    QString make_close(const map_type & properties) const;

    //Generate a comment suitable for putting in the buildfile.
    //
    //This uses a template labelled "comment", splitting text into
    //separate lines and feeding the template a line at a time. If
    //no template exists, then the fall-back is to use 
    //constants::comment_text
    //
    //\param text : text to comment
    
    QString make_comment_text(const QString & text) const;

    //Adds/Checks the date section
    //
    //* Using this method instead of directly accessing the templates is
    //recommended as it combines the passed properties to the build
    //style properties before instantiating the template.
    QString make_date(const map_type & properties) const;

    //Instantiate the named template with the given properties and build
    //properties.  Returns an empty string if the template
    //does not exist.
    //
    //\param properties : map of replacement properties for the template
    //\param tmpl : the name of the template to use
    //
    //* Using this method instead of directly accessing the templates is
    //recommended as it combines the passed properties to the build
    //style properties before instantiating the template.
    
    QString make_element(QString tmpl, const map_type & properties) const;

    //Marker text indicating end of a compound document section with the given name
    //
    //This uses a template labelled "end-phrase". If
    //no template exists, then the fall-back is to use 
    //comment_text( " END:" + name )
    QString make_end_phrase(QString name) const;

    //Adds/Checks the preamble section
    //
    //* Using this method instead of directly accessing the templates is
    //recommended as it combines the passed properties to the build
    //style properties before instantiating the template.
    QString make_preamble(const map_type & properties) const;

    //Marker text indicating beginning of a compound document section with the  given name
    //
    //This uses a template labelled "start-phrase". If
    //no template exists, then the fall-back is to use 
    //comment_text( " START:" + name )
    QString make_start_phrase(QString name) const;

    //Adds/Checks the version section
    //
    //* Using this method instead of directly accessing the templates is
    //recommended as it combines the passed properties to the build
    //style properties before instantiating the template.
    QString make_version(const map_type & properties) const;

};

} // namespace butter
#endif

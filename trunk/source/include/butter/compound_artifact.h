#ifndef BUTTER_COMPOUND_ARTIFACT_HPP
#define BUTTER_COMPOUND_ARTIFACT_HPP


#include <qstring.h>
#include <qdict.h>
#include <qvaluelist.h>
#include <qtextstream.h>

#include "butter/config.h"
// Manual includes
#include <utility>
// --
namespace butter { class butter_constants; } 
namespace butter { class pathcmp; } 
namespace butter { class style; } 
class UmlArtifact;
namespace butter { struct basic_style; } 

namespace butter {

/**
 * An artifact that is made up of multiple-parts. It exposes an ordered list of 
 * pair< string, string > that represent the parts of the artifact. These can
 * be unlabelled or labelled depending on the content of the first entry in the
 * pair.
 * 
 * * labelled parts are enclosed in "start_phrase"/"end_phrase" markers.
 * * unlabelled parts are any non-whitespace outside part markers.
 * 
 * Standard build-file form is a compound_artifact with something like
 * [version] version information
 * [date] date
 * [preamble] ...
 * [target:....] ...
 * [close] ...
 * 
 * * destructor writes parts to artifact.
 */
class compound_artifact
{
  public:
    typedef std::pair< QString, QString > string_pair_t;

    typedef std::pair< QString, string_pair_t > value_type;

    /**
     * Reference to the object this compound artifact is representing.
     */
    ::UmlArtifact & artifact;

    /**
     * The close texts
     */
    string_pair_t close;

    /**
     * The date texts
     */
    string_pair_t date;

    /**
     * Any text at the end of the document.
     */
    QString end;

    /**
     * The preamble texts
     */
    string_pair_t preamble;


  private:
    /**
     * reference to style object
     */
    const basic_style& style_;

    /**
     * The artifact's target parts.
     */
    QDict< string_pair_t > targets_;

    /**
     * The list of key in encounter-order.
     */
    QValueList< QString > target_order_;


  public:
    /**
     * The version texts.
     */
    string_pair_t version;

    /**
     * Construct and initialise object from a_art
     */
    compound_artifact(::UmlArtifact & a_art);

    /**
     * Construct and initialise object from a_art
     */
    compound_artifact(::UmlArtifact & a_art, const basic_style & a_style);

    ~compound_artifact();


  private:
    /**
     * no copy
     */
    compound_artifact(const compound_artifact & source);

    /**
     * Reset object and read-in a new artifact.
     * 
     * The version, date, preamble and close are read as-is. Unlabelled sections preceding
     * targets are read, but the content of the target section is left blank.
     */
    void deserialise(QTextIStream & a_is);


  public:
    /**
     * Check for the existance of a target.
     */
    bool has_target(QString a_label) const;

    /**
     * Merges the content of a_art into this artifact.
     * 
     * Merge process:
     * * Create temporary compound_artifact from a_art.
     * * If any labelled sections exist in both artifacts then the content 
     * from a_art is appended. If an unlabelled section exists before the
     * labelled section then is is appended to any preceding unlabelled
     * section or inserted if no existing labelled section exists.
     * * If any sections remain then they are appended inorder to this 
     * artifact. 
     */
    void merge(::UmlArtifact & a_art);


  private:
    /**
     * no assign
     */
    compound_artifact & operator=(const compound_artifact & source);


  public:
    /**
     * Emptys any target, preamble and close labelled sections
     */
    void reset();

    /**
     * Output the parts into a_os
     */
    void serialise(QTextStream & a_os) const;

    /**
     * Get a reference to the text for target with a_label. Insert if necessary
     */
    string_pair_t & target(QString a_label);


};


} // namespace butter
#endif

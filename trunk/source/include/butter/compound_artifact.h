#ifndef BUTTER_COMPOUND_ARTIFACT_HPP
#define BUTTER_COMPOUND_ARTIFACT_HPP


#include <qstring.h>
#include <memory>

// Manual includes
#include <utility>
// --
namespace butter { class basic_style; } 
namespace butter { class butter_constants; } 
namespace butter { class pathcmp; } 
namespace butter { class style; } 
class UmlArtifact;
namespace butter { class compound_document; } 

namespace butter {

/**
 * An artifact that is made up of multiple-parts. It exposes an ordered list of 
 * pair< string, string > that represent the parts of the artifact. These can
 * be unlabelled or labelled depending on the content of the first entry in the
 * pair.
 * 
 * * labelled parts are enclosed in "start_phrase_"/"end_phrase_" markers.
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
    ::UmlArtifact & artifact_;


  private:
    /**
     * The representation of the description as a compound document.
     */
    std::unique_ptr< compound_document > doc_;

    /**
     * reference to style object
     */
    const basic_style& style_;


  public:
    /**
     * Construct and initialise object from a_art
     */
    compound_artifact(::UmlArtifact & a_art);
    /**
     * Construct and initialise object from a_art
     */
    compound_artifact(::UmlArtifact & a_art, const basic_style & a_style)
    : artifact_( a_art )
    , doc_()
    , style_( a_style )
    {}

    ~compound_artifact();


  private:
    /**
     * no copy
     */
    compound_artifact(const compound_artifact & source) = delete;

    /**
     * no copy
     */
    compound_artifact(compound_artifact && source) = delete;

    /**
     * no assign
     */
    compound_artifact & operator=(const compound_artifact & source) = delete;


  public:
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

    /**
     *  Access the artifact's description as a compound document.
     */
    compound_document & document();


};


} // namespace butter
#endif

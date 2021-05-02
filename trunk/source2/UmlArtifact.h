#ifndef _UMLARTIFACT_H
#define _UMLARTIFACT_H


#include "UmlBaseArtifact.h"
#include <qbytearray.h>
#include <qstring.h>
#include <memory>

namespace butter { class enumerate_path; } 
namespace butter { class compound_document; } 

// This class manages 'artifacts'
//
// You can modify it as you want (except the constructor)
class UmlArtifact : public UmlBaseArtifact {
  public:
    UmlArtifact(void * id, const QByteArray & n) : UmlBaseArtifact(id, n) {};

    //If the compound document of this artifact has changed, update the description.
    ~UmlArtifact();
    //If this artifact has stereotype "document" write to the corresponding file,
    //if the text has changed.
    //
    //\throw : runtime_error if file can not be openned for reading or writing
    //
    //\arg full_path : path of directory containing this artifact
    //\arg log : output for user
    void write_out(const butter::enumerate_path & full_path, butter::base_log & log);

    //Merges the content of a_art into this artifact.
    //
    //Merge process:
    //* Create temporary compound_artifact from a_art.
    //* If any labelled sections exist in both artifacts then the content 
    //from a_art is appended. If an unlabelled section exists before the
    //labelled section then is is appended to any preceding unlabelled
    //section or inserted if no existing labelled section exists.
    //* If any sections remain then they are appended inorder to this 
    //artifact. 
    void merge(UmlArtifact & art);

    // Access the artifact's description as a compound document.
    butter::compound_document & document(QString start_phrase, QString end_phrase);

  private:
    std::unique_ptr<butter::compound_document> doc_;

};

#endif

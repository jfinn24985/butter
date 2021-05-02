
#include "UmlPackage.h"
#include "UmlItem.h"

//Find UmlArtifact objects of the given name at the current location. This
//may return an empty list if none are found
//
//\pre not name.empty
void UmlPackage::find_uml_document(QString name, QVector< UmlItem* > & result) {
  BUTTER_REQUIRE(! name.isEmpty (), "Cannot look for a document without a name");  
  // Only check our deployment views.
  for( UmlItem * deploy_cursor_ : this->children() )
  {
    if( aDeploymentView == deploy_cursor_->kind() )
    {
      for( UmlItem * artifact_cursor_ : deploy_cursor_->children() )
      {
        if (anArtifact == artifact_cursor_->kind()
            && name == artifact_cursor_->name().data()
            && constants::document_stereotype == artifact_cursor_->stereotype().data())
        {
          result.push_back( artifact_cursor_ );
        }
      }
    }
  }
  
}


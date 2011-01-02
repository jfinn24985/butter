#ifndef _UMLARTIFACT_H
#define _UMLARTIFACT_H


#include "bouml/UmlBaseArtifact.h"
#include <qcstring.h>
#include "bouml/UmlPackage.h"

/**
 *  This class manages 'artifacts'
 * 
 *  You can modify it as you want (except the constructor)
 */
class UmlArtifact : public UmlBaseArtifact {
  public:
    UmlArtifact(void * id, const QCString & n) : UmlBaseArtifact(id, n) {};

    /**
     * Get the parent package of this artifact
     * 
     * \pre a_art.parent.parent /= UmlPackage
     */
    const UmlPackage& package() const
    {
      UmlPackage *Result = dynamic_cast< UmlPackage* >(const_cast< UmlArtifact* >(this)->parent ()->parent ());
      // BUTTER_REQUIRE (NULL != Result, "Invalid heirarchy, grandparent of artifact is not a package");
      return *Result;
    }
    
    

};

#endif

#ifndef _UMLEXTRAARTIFACTDEFINITION_H
#define _UMLEXTRAARTIFACTDEFINITION_H


#include "bouml/UmlBaseExtraArtifactDefinition.h"
#include <qbytearray.h>

class UmlExtraArtifactDefinition : public UmlBaseExtraArtifactDefinition {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlExtraArtifactDefinition(void * id, const QByteArray & s) : UmlBaseExtraArtifactDefinition(id, s) {
    }

};
#endif

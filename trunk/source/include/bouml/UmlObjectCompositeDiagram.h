#ifndef _UMLOBJECTCOMPOSITEDIAGRAM_H
#define _UMLOBJECTCOMPOSITEDIAGRAM_H


#include "bouml/UmlBaseObjectCompositeDiagram.h"
#include <qbytearray.h>

class UmlObjectCompositeDiagram : public UmlBaseObjectCompositeDiagram {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlObjectCompositeDiagram(void * id, const QByteArray & s) : UmlBaseObjectCompositeDiagram(id, s) {
    }

};
#endif

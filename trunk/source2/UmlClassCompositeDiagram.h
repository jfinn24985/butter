#ifndef _UMLCLASSCOMPOSITEDIAGRAM_H
#define _UMLCLASSCOMPOSITEDIAGRAM_H


#include "UmlBaseClassCompositeDiagram.h"
#include <qbytearray.h>

class UmlClassCompositeDiagram : public UmlBaseClassCompositeDiagram {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlClassCompositeDiagram(void * id, const QByteArray & s) : UmlBaseClassCompositeDiagram(id, s) {
    }

};
#endif

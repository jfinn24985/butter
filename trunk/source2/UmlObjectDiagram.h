#ifndef _UMLOBJECTDIAGRAM_H
#define _UMLOBJECTDIAGRAM_H


#include "UmlBaseObjectDiagram.h"
#include <qbytearray.h>

// This class manages 'object diagrams', notes that the class 'UmlDiagram'
// is a mother class of all the diagrams, allowing to generalize their
// management
//
// You can modify it as you want (except the constructor)
class UmlObjectDiagram : public UmlBaseObjectDiagram {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlObjectDiagram(void * id, const QByteArray & s) : UmlBaseObjectDiagram(id, s) {
    }

};

#endif

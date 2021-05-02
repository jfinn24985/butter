#ifndef _UMLBASECLASSCOMPOSITEDIAGRAM_H
#define _UMLBASECLASSCOMPOSITEDIAGRAM_H


#include "UmlDiagram.h"
#include "anItemKind.h"
#include <qbytearray.h>

class UmlClassCompositeDiagram;
class UmlClass;

class UmlBaseClassCompositeDiagram : public UmlDiagram {
  public:
    //  returns a new class composite diagram named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlClassCompositeDiagram * create(UmlClass * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseClassCompositeDiagram(void * id, const QByteArray & s) : UmlDiagram(id, s) {
    }

};
#endif

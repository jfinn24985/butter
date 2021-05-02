#ifndef _UMLBASEOBJECTCOMPOSITEDIAGRAM_H
#define _UMLBASEOBJECTCOMPOSITEDIAGRAM_H


#include "UmlDiagram.h"
#include "anItemKind.h"
#include <qbytearray.h>

class UmlObjectCompositeDiagram;
class UmlClassInstance;

class UmlBaseObjectCompositeDiagram : public UmlDiagram {
  public:
    //  returns a new object composite diagram named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlObjectCompositeDiagram * create(UmlClassInstance * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseObjectCompositeDiagram(void * id, const QByteArray & s) : UmlDiagram(id, s) {
    }

};
#endif

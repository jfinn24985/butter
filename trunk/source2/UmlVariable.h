#ifndef _UMLVARIABLE_H
#define _UMLVARIABLE_H


#include "UmlBaseVariable.h"
#include <qbytearray.h>

class UmlVariable : public UmlBaseVariable {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlVariable(void * id, const QByteArray & s) : UmlBaseVariable(id, s) {
    }

};
#endif

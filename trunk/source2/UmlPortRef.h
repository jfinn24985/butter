#ifndef _UMLPORTREF_H
#define _UMLPORTREF_H


#include "UmlBasePortRef.h"
#include <qbytearray.h>

class UmlPortRef : public UmlBasePortRef {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlPortRef(void * id, const QByteArray & s) : UmlBasePortRef(id, s) {
    }

};
#endif

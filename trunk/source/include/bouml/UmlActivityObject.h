#ifndef _UMLACTIVITYOBJECT_H
#define _UMLACTIVITYOBJECT_H


#include "bouml/UmlBaseActivityObject.h"
#include <qbytearray.h>

class UmlActivityObject : public UmlBaseActivityObject {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlActivityObject(void * id, const QByteArray & s) : UmlBaseActivityObject(id, s) {
    }

};

#endif

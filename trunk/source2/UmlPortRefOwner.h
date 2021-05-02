#ifndef _UMLPORTREFOWNER_H
#define _UMLPORTREFOWNER_H


#include "UmlConnectable.h"
#include <qbytearray.h>

class UmlPortRefOwner : public UmlConnectable {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlPortRefOwner(void * id, const QByteArray & s) : UmlConnectable(id, s) {
    }

};
#endif

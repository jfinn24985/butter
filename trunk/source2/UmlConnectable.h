#ifndef _UMLCONNECTABLE_H
#define _UMLCONNECTABLE_H


#include "UmlItem.h"
#include <qbytearray.h>

class UmlConnectable : public UmlItem {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlConnectable(void * id, const QByteArray & s) : UmlItem(id, s) {
    }

};
#endif

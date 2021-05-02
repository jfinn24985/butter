#ifndef _UMLPORT_H
#define _UMLPORT_H


#include "bouml/UmlBasePort.h"
#include <qbytearray.h>

class UmlPort : public UmlBasePort {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlPort(void * id, const QByteArray & s) : UmlBasePort(id, s) {
    }

};
#endif

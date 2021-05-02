#ifndef _UMLCONNECTOR_H
#define _UMLCONNECTOR_H


#include "UmlBaseConnector.h"
#include <qbytearray.h>

class UmlConnector : public UmlBaseConnector {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlConnector(void * id, const QByteArray & s) : UmlBaseConnector(id, s) {
    }

};
#endif

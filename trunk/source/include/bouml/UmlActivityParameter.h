#ifndef _UMLACTIVITYPARAMETER_H
#define _UMLACTIVITYPARAMETER_H


#include "bouml/UmlBaseActivityParameter.h"
#include <qbytearray.h>

class UmlActivityParameter : public UmlBaseActivityParameter {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlActivityParameter(void * id, const QByteArray & s) : UmlBaseActivityParameter(id, s) {
    }

};

#endif

#ifndef _UMLPARAMETERSET_H
#define _UMLPARAMETERSET_H


#include "bouml/UmlBaseParameterSet.h"
#include "bouml/UmlActivityItem.h"
#include <qbytearray.h>

class UmlParameterSet : public UmlBaseParameterSet, public UmlActivityItem {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlParameterSet(void * id, const QByteArray & s) : UmlBaseParameterSet(id, s) {
    }

};

#endif

#ifndef _UMLFLOW_H
#define _UMLFLOW_H


#include "bouml/UmlBaseFlow.h"
#include "bouml/UmlActivityItem.h"
#include <qbytearray.h>

class UmlFlow : public UmlBaseFlow, public UmlActivityItem {
  public:
     UmlFlow(void * id, const QByteArray & n) : UmlBaseFlow(id, n) {
    }

};

#endif

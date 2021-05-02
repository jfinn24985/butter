#ifndef _UMLTRANSITION_H
#define _UMLTRANSITION_H


#include "bouml/UmlBaseTransition.h"
#include "bouml/UmlStateItem.h"
#include <qbytearray.h>

class UmlTransition : public UmlBaseTransition, public UmlStateItem {
  public:
     UmlTransition(void * id, const QByteArray & n) : UmlBaseTransition(id, n) {
    }

};

#endif

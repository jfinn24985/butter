#ifndef _UMLACTIVITYREGION_H
#define _UMLACTIVITYREGION_H


#include "bouml/UmlBaseActivityRegion.h"
#include "bouml/UmlActivityItem.h"
#include <qbytearray.h>

class UmlActivityRegion : public UmlBaseActivityRegion, public UmlActivityItem {
  protected:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlActivityRegion(void * id, const QByteArray & s) : UmlBaseActivityRegion(id, s) {
    }

};

#endif

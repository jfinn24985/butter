#ifndef _UMLPSEUDOSTATE_H
#define _UMLPSEUDOSTATE_H


#include <qbytearray.h>
#include "bouml/UmlItem.h"
#include "bouml/UmlStateItem.h"

class UmlPseudoState : public UmlStateItem, public UmlItem {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlPseudoState(void * id, const QByteArray & s) : UmlItem(id, s) {
    }

};

#endif

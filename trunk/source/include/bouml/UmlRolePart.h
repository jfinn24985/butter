#ifndef _UMLROLEPART_H
#define _UMLROLEPART_H


#include "bouml/UmlBaseRolePart.h"
#include <qbytearray.h>

class UmlRolePart : public UmlBaseRolePart {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlRolePart(void * id, const QByteArray & s) : UmlBaseRolePart(id, s) {
    }

};
#endif

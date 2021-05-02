#ifndef _UMLROLEPARTINSTANCE_H
#define _UMLROLEPARTINSTANCE_H


#include "bouml/UmlBaseRolePartInstance.h"
#include <qbytearray.h>

class UmlRolePartInstance : public UmlBaseRolePartInstance {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlRolePartInstance(void * id, const QByteArray & s) : UmlBaseRolePartInstance(id, s) {
    }

};
#endif

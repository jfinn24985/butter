#ifndef _UMLPINPARAMETER_H
#define _UMLPINPARAMETER_H


#include "bouml/UmlBasePinParameter.h"
#include <qbytearray.h>

class UmlPinParameter : public UmlBasePinParameter {
  protected:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlPinParameter(void * id, const QByteArray & s) : UmlBasePinParameter(id, s) {
    }

};

#endif

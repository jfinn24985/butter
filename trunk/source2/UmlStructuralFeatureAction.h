#ifndef _UMLSTRUCTURALFEATUREACTION_H
#define _UMLSTRUCTURALFEATUREACTION_H


#include "UmlBaseStructuralFeatureAction.h"
#include <qbytearray.h>

class UmlStructuralFeatureAction : public UmlBaseStructuralFeatureAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlStructuralFeatureAction(void * id, const QByteArray & s) : UmlBaseStructuralFeatureAction(id, s) {
    }

};
#endif

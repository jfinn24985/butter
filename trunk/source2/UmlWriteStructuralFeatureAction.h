#ifndef _UMLWRITESTRUCTURALFEATUREACTION_H
#define _UMLWRITESTRUCTURALFEATUREACTION_H


#include "UmlBaseWriteStructuralFeatureAction.h"
#include <qbytearray.h>

class UmlWriteStructuralFeatureAction : public UmlBaseWriteStructuralFeatureAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlWriteStructuralFeatureAction(void * id, const QByteArray & s) : UmlBaseWriteStructuralFeatureAction(id, s) {
    }

};
#endif

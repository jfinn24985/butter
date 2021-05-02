#ifndef _UMLBASEWRITESTRUCTURALFEATUREACTION_H
#define _UMLBASEWRITESTRUCTURALFEATUREACTION_H


#include "UmlStructuralFeatureAction.h"
#include <qbytearray.h>

class UmlBaseWriteStructuralFeatureAction : public UmlStructuralFeatureAction {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseWriteStructuralFeatureAction(void * id, const QByteArray & s) : UmlStructuralFeatureAction(id, s) {
    }

};
#endif

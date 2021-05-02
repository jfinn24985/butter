#ifndef _UMLBASESTRUCTURALFEATUREACTION_H
#define _UMLBASESTRUCTURALFEATUREACTION_H


#include "UmlActivityAction.h"
#include <qbytearray.h>

class UmlItem;

class UmlBaseStructuralFeatureAction : public UmlActivityAction {
  public:
    // return the structural feature
    UmlItem * structuralFeature();

    // set the structural feature
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_StructuralFeature(UmlItem * v);


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseStructuralFeatureAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }

    void read_uml_();


  private:
    UmlItem * _structural_feature;

};
#endif

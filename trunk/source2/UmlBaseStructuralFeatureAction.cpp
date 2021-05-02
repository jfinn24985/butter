
#include "UmlCom.h"
#include "UmlBaseStructuralFeatureAction.h"
#include "UmlItem.h"

// return the structural feature
UmlItem * UmlBaseStructuralFeatureAction::structuralFeature() {
    read_if_needed_();
    return _structural_feature;
}

// set the structural feature
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseStructuralFeatureAction::set_StructuralFeature(UmlItem * v) {
    UmlCom::send_cmd(_identifier, setDefCmd, (v == 0) ? (void *) v : ((UmlBaseItem *) v)->_identifier);
    if (UmlCom::read_bool()) {
      _structural_feature = v;
      return TRUE;
    }
    else
      return FALSE;
}

void UmlBaseStructuralFeatureAction::read_uml_() {
    UmlBaseActivityAction::read_uml_();
    _structural_feature = UmlBaseItem::read_();
}


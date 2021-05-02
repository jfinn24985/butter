
#include "UmlCom.h"
#include "UmlBaseEntryPointPseudoState.h"
#include "UmlEntryPointPseudoState.h"
#include "UmlItem.h"

UmlEntryPointPseudoState * UmlBaseEntryPointPseudoState::create(UmlItem * parent, const char * s)
{
  return (UmlEntryPointPseudoState *) parent->create_(anEntryPointPseudoState, s);
}

anItemKind UmlBaseEntryPointPseudoState::kind() {
  return anEntryPointPseudoState;
}

// return the the referenced sub machine state or 0/null
// if the state is not a sub machine state reference
UmlEntryPointPseudoState * UmlBaseEntryPointPseudoState::reference() {
    read_if_needed_();
    return _reference;
}

// set the referenced sub machine state (may be 0/null)
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseEntryPointPseudoState::set_Reference(UmlEntryPointPseudoState * v) {
    UmlCom::send_cmd(_identifier, setDerivedCmd, (v == 0) ? (void *) v : ((UmlBaseItem *) v)->_identifier);
    if (UmlCom::read_bool()) {
      _reference = v;
      return TRUE;
    }
    else
      return FALSE;
}

void UmlBaseEntryPointPseudoState::read_uml_() {
    UmlBaseItem::read_uml_();
    _reference = (UmlEntryPointPseudoState *) UmlBaseItem::read_();
}


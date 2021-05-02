
#include "UmlCom.h"
#include "UmlBaseExitPointPseudoState.h"
#include "UmlExitPointPseudoState.h"
#include "UmlItem.h"

UmlExitPointPseudoState * UmlBaseExitPointPseudoState::create(UmlItem * parent, const char * s)
{
  return (UmlExitPointPseudoState *) parent->create_(anExitPointPseudoState, s);
}

anItemKind UmlBaseExitPointPseudoState::kind() {
  return anExitPointPseudoState;
}

// return the the referenced sub machine state or 0/null
// if the state is not a sub machine state reference
UmlExitPointPseudoState * UmlBaseExitPointPseudoState::reference() {
    read_if_needed_();
    return _reference;
}

// set the referenced sub machine state (may be 0/null)
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseExitPointPseudoState::set_Reference(UmlExitPointPseudoState * v) {
    UmlCom::send_cmd(_identifier, setDerivedCmd, (v == 0) ? (void *) v : ((UmlBaseItem *) v)->_identifier);
    if (UmlCom::read_bool()) {
      _reference = v;
      return TRUE;
    }
    else
      return FALSE;
}

void UmlBaseExitPointPseudoState::read_uml_() {
    UmlBaseItem::read_uml_();
    _reference = (UmlExitPointPseudoState *) UmlBaseItem::read_();
}


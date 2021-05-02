
#include "bouml/UmlBaseTerminatePseudoState.h"
#include "bouml/UmlTerminatePseudoState.h"
#include "bouml/UmlItem.h"

UmlTerminatePseudoState * UmlBaseTerminatePseudoState::create(UmlItem * parent)
{
  return (UmlTerminatePseudoState *) parent->create_(aTerminatePseudoState, "");
}

anItemKind UmlBaseTerminatePseudoState::kind() {
  return aTerminatePseudoState;
}


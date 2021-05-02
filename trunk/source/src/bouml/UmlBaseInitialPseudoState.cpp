
#include "bouml/UmlBaseInitialPseudoState.h"
#include "bouml/UmlInitialPseudoState.h"
#include "bouml/UmlItem.h"

UmlInitialPseudoState * UmlBaseInitialPseudoState::create(UmlItem * parent)
{
  return (UmlInitialPseudoState *) parent->create_(anInitialPseudoState, "");
}

anItemKind UmlBaseInitialPseudoState::kind() {
  return anInitialPseudoState;
}


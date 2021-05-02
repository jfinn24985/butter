
#include "bouml/UmlBaseChoicePseudoState.h"
#include "bouml/UmlChoicePseudoState.h"
#include "bouml/UmlItem.h"

UmlChoicePseudoState * UmlBaseChoicePseudoState::create(UmlItem * parent)
{
  return (UmlChoicePseudoState *) parent->create_(aChoicePseudoState, "");
}

anItemKind UmlBaseChoicePseudoState::kind() {
  return aChoicePseudoState;
}


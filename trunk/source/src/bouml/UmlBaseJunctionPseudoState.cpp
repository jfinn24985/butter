
#include "bouml/UmlBaseJunctionPseudoState.h"
#include "bouml/UmlJunctionPseudoState.h"
#include "bouml/UmlItem.h"

UmlJunctionPseudoState * UmlBaseJunctionPseudoState::create(UmlItem * parent)
{
  return (UmlJunctionPseudoState *) parent->create_(aJunctionPseudoState, "");
}

anItemKind UmlBaseJunctionPseudoState::kind() {
  return aJunctionPseudoState;
}



#include "bouml/UmlBaseJoinPseudoState.h"
#include "bouml/UmlJoinPseudoState.h"
#include "bouml/UmlItem.h"

UmlJoinPseudoState * UmlBaseJoinPseudoState::create(UmlItem * parent)
{
  return (UmlJoinPseudoState *) parent->create_(aJoinPseudoState, "");
}

anItemKind UmlBaseJoinPseudoState::kind() {
  return aJoinPseudoState;
}


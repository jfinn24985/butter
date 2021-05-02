
#include "bouml/UmlBaseShallowHistoryPseudoState.h"
#include "bouml/UmlShallowHistoryPseudoState.h"
#include "bouml/UmlItem.h"

UmlShallowHistoryPseudoState * UmlBaseShallowHistoryPseudoState::create(UmlItem * parent)
{
  return (UmlShallowHistoryPseudoState *) parent->create_(aShallowHistoryPseudoState, "");
}

anItemKind UmlBaseShallowHistoryPseudoState::kind() {
  return aShallowHistoryPseudoState;
}


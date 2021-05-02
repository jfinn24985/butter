
#include "bouml/UmlBaseDeepHistoryPseudoState.h"
#include "bouml/UmlDeepHistoryPseudoState.h"
#include "bouml/UmlItem.h"

UmlDeepHistoryPseudoState * UmlBaseDeepHistoryPseudoState::create(UmlItem * parent)
{
  return (UmlDeepHistoryPseudoState *) parent->create_(aDeepHistoryPseudoState, "");
}

anItemKind UmlBaseDeepHistoryPseudoState::kind() {
  return aDeepHistoryPseudoState;
}


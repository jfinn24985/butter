
#include "bouml/UmlBaseFinalState.h"
#include "bouml/UmlFinalState.h"
#include "bouml/UmlItem.h"

UmlFinalState * UmlBaseFinalState::create(UmlItem * parent)
{
  return (UmlFinalState *) parent->create_(aFinalState, "");
}

anItemKind UmlBaseFinalState::kind() {
  return aFinalState;
}


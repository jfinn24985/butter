
#include "bouml/UmlBaseForkPseudoState.h"
#include "bouml/UmlForkPseudoState.h"
#include "bouml/UmlItem.h"

UmlForkPseudoState * UmlBaseForkPseudoState::create(UmlItem * parent)
{
  return (UmlForkPseudoState *) parent->create_(aForkPseudoState, "");
}

anItemKind UmlBaseForkPseudoState::kind() {
  return aForkPseudoState;
}


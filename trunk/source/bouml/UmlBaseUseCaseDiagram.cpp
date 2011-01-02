
#include "bouml/UmlBaseUseCaseDiagram.h"
#include "bouml/UmlUseCaseDiagram.h"
#include "bouml/UmlItem.h"

UmlUseCaseDiagram * UmlBaseUseCaseDiagram::create(UmlItem * parent, const char * s)
{
  return (UmlUseCaseDiagram *) parent->create_(anUseCaseDiagram, s);
}

anItemKind UmlBaseUseCaseDiagram::kind() {
  return anUseCaseDiagram;
}


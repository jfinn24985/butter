
#include "bouml/UmlBaseStateDiagram.h"
#include "bouml/UmlStateDiagram.h"
#include "bouml/UmlState.h"

UmlStateDiagram * UmlBaseStateDiagram::create(UmlState * parent, const char * s)
{
  return (UmlStateDiagram *) parent->create_(aStateDiagram, s);
}

anItemKind UmlBaseStateDiagram::kind() {
  return aStateDiagram;
}


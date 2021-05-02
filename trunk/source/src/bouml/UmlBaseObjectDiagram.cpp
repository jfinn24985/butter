
#include "bouml/UmlBaseObjectDiagram.h"
#include "bouml/UmlObjectDiagram.h"
#include "bouml/UmlItem.h"

UmlObjectDiagram * UmlBaseObjectDiagram::create(UmlItem * parent, const char * s)
{
  return (UmlObjectDiagram *) parent->create_(anObjectDiagram, s);
}

anItemKind UmlBaseObjectDiagram::kind() {
  return anObjectDiagram;
}


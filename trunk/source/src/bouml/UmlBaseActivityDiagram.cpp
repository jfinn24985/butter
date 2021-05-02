
#include "bouml/UmlBaseActivityDiagram.h"
#include "bouml/UmlActivityDiagram.h"
#include "bouml/UmlItem.h"

UmlActivityDiagram * UmlBaseActivityDiagram::create(UmlItem * parent, const char * s)
{
  return (UmlActivityDiagram *) parent->create_(anActivityDiagram, s);
}

anItemKind UmlBaseActivityDiagram::kind() {
  return anActivityDiagram;
}


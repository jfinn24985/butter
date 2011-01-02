
#include "bouml/UmlBaseComponentDiagram.h"
#include "bouml/UmlComponentDiagram.h"
#include "bouml/UmlComponentView.h"

UmlComponentDiagram * UmlBaseComponentDiagram::create(UmlComponentView * parent, const char * s)
{
  return (UmlComponentDiagram *) parent->create_(aComponentDiagram, s);
}

anItemKind UmlBaseComponentDiagram::kind() {
  return aComponentDiagram;
}


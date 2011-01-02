
#include "bouml/UmlBaseClassDiagram.h"
#include "bouml/UmlClassDiagram.h"
#include "bouml/UmlClassView.h"

UmlClassDiagram * UmlBaseClassDiagram::create(UmlClassView * parent, const char * s)
{
  return (UmlClassDiagram *) parent->create_(aClassDiagram, s);
}

anItemKind UmlBaseClassDiagram::kind() {
  return aClassDiagram;
}


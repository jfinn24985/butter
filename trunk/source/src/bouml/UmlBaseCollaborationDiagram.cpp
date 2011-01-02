
#include "bouml/UmlBaseCollaborationDiagram.h"
#include "bouml/UmlCollaborationDiagram.h"
#include "bouml/UmlItem.h"

UmlCollaborationDiagram * UmlBaseCollaborationDiagram::create(UmlItem * parent, const char * s)
{
  return (UmlCollaborationDiagram *) parent->create_(aCollaborationDiagram, s);
}

anItemKind UmlBaseCollaborationDiagram::kind() {
  return aCollaborationDiagram;
}



#include "bouml/UmlBaseDeploymentDiagram.h"
#include "bouml/UmlDeploymentDiagram.h"
#include "bouml/UmlDeploymentView.h"

UmlDeploymentDiagram * UmlBaseDeploymentDiagram::create(UmlDeploymentView * parent, const char * s)
{
  return (UmlDeploymentDiagram *) parent->create_(aDeploymentDiagram, s);
}

anItemKind UmlBaseDeploymentDiagram::kind() {
  return aDeploymentDiagram;
}


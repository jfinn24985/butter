
#include "bouml/UmlBaseDeploymentView.h"
#include "bouml/UmlDeploymentView.h"
#include "bouml/UmlPackage.h"

UmlDeploymentView * UmlBaseDeploymentView::create(UmlPackage * parent, const char * s)
{
  return (UmlDeploymentView *) parent->create_(aDeploymentView, s);
}

anItemKind UmlBaseDeploymentView::kind() {
  return aDeploymentView;
}


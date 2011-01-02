
#include "bouml/UmlBaseComponentView.h"
#include "bouml/UmlComponentView.h"
#include "bouml/UmlPackage.h"

UmlComponentView * UmlBaseComponentView::create(UmlPackage * parent, const char * s)
{
  return (UmlComponentView *) parent->create_(aComponentView, s);
}

anItemKind UmlBaseComponentView::kind() {
  return aComponentView;
}


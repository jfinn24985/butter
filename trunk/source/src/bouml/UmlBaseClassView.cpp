
#include "bouml/UmlBaseClassView.h"
#include "bouml/UmlClassView.h"
#include "bouml/UmlPackage.h"
#include "bouml/UmlDeploymentView.h"

#include "UmlCom.h"
UmlClassView * UmlBaseClassView::create(UmlPackage * parent, const char * s)
{
  return (UmlClassView *) parent->create_(aClassView, s);
}

anItemKind UmlBaseClassView::kind() {
  return aClassView;
}

UmlDeploymentView * UmlBaseClassView::associatedDeploymentView() {
  read_if_needed_();
  
  return _assoc_view;
}

bool UmlBaseClassView::set_AssociatedDeploymentView(UmlDeploymentView * v) {
  UmlCom::send_cmd(_identifier, setAssocViewCmd, ((UmlBaseItem *) v)->_identifier);
  if (UmlCom::read_bool()) {
    _assoc_view = v;
    return TRUE;
  }
  else
    return FALSE;
}

void UmlBaseClassView::read_uml_() {
  _assoc_view = (UmlDeploymentView *) UmlBaseItem::read_();
  UmlBaseItem::read_uml_();
}


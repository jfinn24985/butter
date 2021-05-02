
#include "UmlCom.h"
#include "UmlBaseAccessVariableValueAction.h"
#include "UmlVariable.h"

UmlVariable * UmlBaseAccessVariableValueAction::variable() {
  read_if_needed_();
  return _variable;
}

bool UmlBaseAccessVariableValueAction::set_Variable(UmlVariable * v) {
  UmlCom::send_cmd(_identifier, setDefCmd, ((UmlBaseItem *) v)->_identifier);   if (UmlCom::read_bool()) {     _variable = v;     return TRUE;   }   else     return FALSE;
}

void UmlBaseAccessVariableValueAction::read_uml_() {
  UmlBaseActivityAction::read_uml_();
  _variable = (UmlVariable *) UmlBaseItem::read_();
}


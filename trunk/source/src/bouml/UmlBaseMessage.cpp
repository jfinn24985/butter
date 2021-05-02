
#include "UmlCom.h"
#include "bouml/UmlBaseMessage.h"
#include "bouml/UmlClassInstanceReference.h"
#include "bouml/UmlOperation.h"

void UmlBaseMessage::read_(const Q3PtrDict<UmlClassInstanceReference> & instances) {
  _operation = (UmlOperation *) UmlBaseItem::read_();
  if (_operation == 0) _form = UmlCom::read_string();
  _from = instances[(void *) UmlCom::read_unsigned()];
  _to = instances[(void *) UmlCom::read_unsigned()];
}


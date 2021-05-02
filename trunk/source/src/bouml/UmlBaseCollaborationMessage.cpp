
#include "UmlCom.h"
#include "bouml/UmlBaseCollaborationMessage.h"
#include "bouml/UmlClassInstanceReference.h"

void UmlBaseCollaborationMessage::read_(const Q3PtrDict<UmlClassInstanceReference> & instances) {
  UmlBaseMessage::read_(instances);
  _rank = (int) UmlCom::read_unsigned();
  _hrank = UmlCom::read_string();
}



#include "bouml/UmlBaseInterruptibleActivityRegion.h"
#include "bouml/UmlInterruptibleActivityRegion.h"
#include "bouml/UmlItem.h"

UmlInterruptibleActivityRegion * UmlBaseInterruptibleActivityRegion::create(UmlItem * parent, const char * s)
{
  return (UmlInterruptibleActivityRegion *) parent->create_(anInterruptibleActivityRegion, s);
}

anItemKind UmlBaseInterruptibleActivityRegion::kind() {
  return anInterruptibleActivityRegion;
}


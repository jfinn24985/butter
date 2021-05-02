
#include "bouml/UmlBaseActivityPin.h"
#include "bouml/UmlActivityPin.h"
#include "bouml/UmlActivityAction.h"

UmlActivityPin * UmlBaseActivityPin::create(UmlActivityAction * parent, const char * s)
{
  return (UmlActivityPin *) parent->create_(anActivityPin, s);
}

anItemKind UmlBaseActivityPin::kind() {
  return anActivityPin;
}


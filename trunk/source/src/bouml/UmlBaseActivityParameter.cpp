
#include "UmlCom.h"
#include "bouml/UmlBaseActivityParameter.h"
#include "bouml/UmlActivityParameter.h"
#include "bouml/UmlActivity.h"

UmlActivityParameter * UmlBaseActivityParameter::create(UmlActivity * parent, const char * s)
{
  return (UmlActivityParameter *) parent->create_(anActivityParameter, s);
}

anItemKind UmlBaseActivityParameter::kind() {
  return anActivityParameter;
}

const QByteArray & UmlBaseActivityParameter::defaultValue() {
  read_if_needed_();
  return _default_value;
}

bool UmlBaseActivityParameter::set_DefaultValue(const char * v) {
  return set_it_(_default_value, v, setDefaultValueCmd);
}

void UmlBaseActivityParameter::unload(bool rec, bool del) {
  _default_value = 0;
  UmlBaseActivityObject::unload(rec, del);
}

void UmlBaseActivityParameter::read_uml_() {
  UmlBasePinParameter::read_uml_();
  _default_value = UmlCom::read_string();
}



#include "bouml/UmlBaseTypeSpec.h"
#include "bouml/UmlClass.h"

const QByteArray & UmlBaseTypeSpec::toString() const {
  return (type) ? type->name() : explicit_type;
}


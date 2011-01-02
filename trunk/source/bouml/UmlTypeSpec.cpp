
#include "bouml/UmlTypeSpec.h"
#include "bouml/UmlClass.h"

const QCString & UmlTypeSpec::toString() const {
  return (type) ? type->name() : explicit_type;
}


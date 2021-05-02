
#include "UmlCom.h"
#include "UmlBaseRolePart.h"
#include "UmlRolePart.h"
#include "UmlClass.h"

//  returns a new role/part named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlRolePart * UmlBaseRolePart::create(UmlClass * parent, const char * s)
{
    return (UmlRolePart *) parent->create_(aRolePart, s);
}

// returns the kind of the item
anItemKind UmlBaseRolePart::kind() {
    return aRolePart;
}

// return the element is a part if true, else a role
bool UmlBaseRolePart::isPart() {
    read_if_needed_();
    return _isPart;
}

// set set if the element is a part or a role
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseRolePart::set_isPart(bool v) {
    return set_it_(_isPart, v, setIsPartCmd);
}

// return the type
UmlClass * UmlBaseRolePart::type() {
    read_if_needed_();
    return _type;
}

// set the type
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseRolePart::set_Type(UmlClass * v) {
    UmlCom::send_cmd(_identifier, setTypeCmd, (v == 0) ? (void *) v : ((UmlBaseItem *) v)->_identifier);
    if (UmlCom::read_bool()) {
      _type = v;
      return TRUE;
    }
    else
      return FALSE;
}

// return the multiplicity
const QByteArray & UmlBaseRolePart::multiplicity() {
    read_if_needed_();
    return _multiplicity;
}

// set the multiplicity
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseRolePart::set_Multiplicity(const QByteArray v) {
    return set_it_(_multiplicity, v, setMultiplicityCmd);
}

void UmlBaseRolePart::read_uml_() {
  UmlBaseItem::read_uml_();
  _isPart = UmlCom::read_bool();
  _type = (UmlClass *) UmlBaseItem::read_();
  _multiplicity = UmlCom::read_string();
}


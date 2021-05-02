
#include "UmlCom.h"
#include "UmlBaseVariable.h"
#include "UmlVariable.h"

//  returns a new variable named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlVariable * UmlBaseVariable::create(UmlItem * parent, const char * s)
{
    return (UmlVariable *) parent->create_(aVariable, s);
}

// returns the kind of the item
anItemKind UmlBaseVariable::kind() {
    return aVariable;
}

// return the type
const UmlTypeSpec & UmlBaseVariable::type() {
    read_if_needed_();
    return _type;
}

// set the type
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseVariable::set_Type(const UmlTypeSpec v) {
    return set_it_(_type, v, setTypeCmd);
}

// return the multiplicity
const QByteArray & UmlBaseVariable::multiplicity() {
    read_if_needed_();
    return _multiplicity;
}

// set the multiplicity
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseVariable::set_Multiplicity(const QByteArray v) {
    return set_it_(_multiplicity, v, setMultiplicityCmd);
}

// return the property 'ordered'
bool UmlBaseVariable::isOrdered() {
    read_if_needed_();
    return _ordered;
}

// set the property 'ordered'
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseVariable::set_isOrdered(bool v) {
    return set_it_(_ordered, v, setOrderingCmd);
}

// return the property 'unique'
bool UmlBaseVariable::isUnique() {
    read_if_needed_();
    return _unique;
}

// set the property 'unique'
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseVariable::set_isUnique(bool v) {
    return set_it_(_unique, v, setUniqueCmd);
}

// return the visibility
aVisibility UmlBaseVariable::visibility() {
    read_if_needed_();
    return _visibility;
}

// set the visibility
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseVariable::set_Visibility(aVisibility v) {
    UmlCom::send_cmd(_identifier, setVisibilityCmd, (char) v);
    if (UmlCom::read_bool()) {
      _visibility = v;
      return TRUE;
    }
    else
      return FALSE;
}

void UmlBaseVariable::read_uml_() {
    UmlBaseItem::read_uml_();
    _type.type = (UmlClass *) UmlBaseItem::read_();
    if (_type.type == 0)
      _type.explicit_type = UmlCom::read_string();
    _multiplicity = UmlCom::read_string();
    _ordered = UmlCom::read_bool();
    _unique = UmlCom::read_bool();
    _visibility = (aVisibility) UmlCom::read_char();
}


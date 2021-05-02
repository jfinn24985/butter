
#include "UmlCom.h"
#include "UmlBaseRolePartInstance.h"
#include "UmlRolePartInstance.h"
#include "UmlClassInstance.h"
#include "UmlRolePart.h"
#include "UmlAttribute.h"

//  returns a new role/part instance named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlRolePartInstance * UmlBaseRolePartInstance::create(UmlClassInstance * parent, const char * s, UmlRolePart * type)
{
    UmlCom::send_cmd(parent->_identifier, createCmd, aRolePartInstance,
  		   type->_identifier);
    UmlRolePartInstance * result = (UmlRolePartInstance *) UmlBaseItem::read_();
  
    if (result != 0) {
      parent->reread_children_if_needed_();
      if (s != 0) result->set_Name(s);
    }
    return result;
}

// returns the kind of the item
anItemKind UmlBaseRolePartInstance::kind() {
    return aRolePartInstance;
}

// return the role/part typing the instance
UmlRolePart * UmlBaseRolePartInstance::type() {
    read_if_needed_();
    return _type;
}

void UmlBaseRolePartInstance::attributesValue(Q3ValueList<SlotAttribute> & result) {
  UmlCom::send_cmd(_identifier, attributesCmd, (char) 0);

  unsigned n = UmlCom::read_unsigned();

  result.clear();
  while (n--) {
    UmlAttribute * at = (UmlAttribute *) UmlBaseItem::read_();

    result.append(SlotAttribute(at, UmlCom::read_string()));
  }
}

bool UmlBaseRolePartInstance::set_AttributeValue(UmlAttribute * attribute, const char * value) {
  UmlCom::send_cmd(_identifier, setAttributeCmd, attribute->_identifier, value);
  return UmlCom::read_bool();
}

void UmlBaseRolePartInstance::read_uml_() {
  UmlBaseItem::read_uml_();
  _type = (UmlRolePart *) UmlBaseItem::read_();
}


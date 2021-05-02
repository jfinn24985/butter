
#include "UmlCom.h"
#include "bouml/UmlBasePortRef.h"
#include "bouml/UmlPortRef.h"
#include "bouml/UmlPortRefOwner.h"
#include "bouml/UmlPort.h"

/**
 *   returns a new port reference named 's' created under 'parent'
 * 
 *  In case it cannot be created (the name is already used or
 *  invalid, 'parent' cannot contain it etc ...) return 0 in C++
 *  and produce a RuntimeException in Java
 */
UmlPortRef * UmlBasePortRef::create(UmlPortRefOwner * parent, const char * s, UmlPort * port)
{
    UmlCom::send_cmd(parent->_identifier, createCmd, aPortRef,
  		   port->_identifier);
    return (UmlPortRef *) UmlBaseItem::read_();
}

/**
 *  returns the kind of the item
 */
anItemKind UmlBasePortRef::kind() {
    return aPortRef;
}

/**
 *  return the port
 */
UmlPort * UmlBasePortRef::port() {
    read_if_needed_();
    return _port;
}

void UmlBasePortRef::read_uml_() {
  UmlBaseItem::read_uml_();
  _port = (UmlPort *) UmlBaseItem::read_();
}


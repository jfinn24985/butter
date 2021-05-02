
#include "UmlCom.h"
#include "bouml/UmlBaseConnector.h"
#include "bouml/UmlConnector.h"
#include "bouml/UmlConnectable.h"

/**
 *  returns a new connector between 'c1' and 'c2'
 * 
 *  In case it cannot be created return 0 in C++ and produce a RuntimeException in Java
 */
UmlConnector * UmlBaseConnector::create(UmlConnectable * c1, UmlConnectable * c2)
{
    UmlCom::send_cmd(c1->_identifier, createCmd, aConnector, c2->_identifier);
    UmlConnector * result = (UmlConnector *) UmlBaseItem::read_();
    
    if (result != 0) {
      c1->reread_children_if_needed_();
      c2->reread_children_if_needed_();
    }
    
    return result;
}

/**
 *  returns the kind of the item
 */
anItemKind UmlBaseConnector::kind() {
    return aConnector;
}

bool UmlBaseConnector::set_Name(const QByteArray & s) {
  if (UmlBaseItem::set_Name(s)) {
    ((UmlBaseConnector *) UmlBaseItem::read_())->_name = s;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseConnector::set_Stereotype(const QByteArray & s) {
  if (UmlBaseItem::set_Stereotype(s)) {
    UmlBaseConnector * other = (UmlBaseConnector *) UmlBaseItem::read_();
    
    if (other != 0)
      other->_stereotype = s;
    return TRUE;
  }
  else
    return FALSE;
}

/**
 *  return the multiplicity
 */
const QByteArray & UmlBaseConnector::multiplicity() {
    read_if_needed_();
    return _multiplicity;
}

/**
 *  set the multiplicity
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBaseConnector::set_Multiplicity(const QByteArray v) {
    return set_it_(_multiplicity, v, setMultiplicityCmd);
}

/**
 *  return the target of the connector, see also use side()
 */
UmlConnectable * UmlBaseConnector::connectable() {
    read_if_needed_();
    return _connectable;
}

/**
 *  if 'first' is true returns the first side of the connector, else the second
 */
UmlConnector * UmlBaseConnector::side(bool first) {
    UmlCom::send_cmd(_identifier, sideCmd, (char) first);
  
    return (UmlConnector *) UmlBaseItem::read_();
}

void UmlBaseConnector::read_uml_() {
  UmlBaseItem::read_uml_();
  _multiplicity = UmlCom::read_string();
  _connectable = (UmlConnectable *) UmlBaseItem::read_();
}


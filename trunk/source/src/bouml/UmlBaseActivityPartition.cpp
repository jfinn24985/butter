
#include "UmlCom.h"
#include "bouml/UmlBaseActivityPartition.h"
#include "bouml/UmlActivityPartition.h"
#include "bouml/UmlActivityDiagram.h"

/**
 *   returns a new activity partition named 's' created under 'parent'
 * 
 *  In case it cannot be created (the name is already used or
 *  invalid, 'parent' cannot contain it etc ...) return 0 in C++
 *  and produce a RuntimeException in Java
 */
UmlActivityPartition * UmlBaseActivityPartition::create(UmlItem * parent, const char * s)
{
    return (UmlActivityPartition *) parent->create_(aPartition, s);
}

/**
 *  returns the kind of the item
 */
anItemKind UmlBaseActivityPartition::kind() {
    return aPartition;
}

/**
 *  returns the optional associated diagram
 */
UmlActivityDiagram * UmlBaseActivityPartition::associatedDiagram() {
    read_if_needed_();
  
    return _assoc_diagram;
}

/**
 *  sets the associated diagram, arg may be null to unset it
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBaseActivityPartition::set_AssociatedDiagram(UmlActivityDiagram * d) {
    UmlCom::send_cmd(_identifier, setAssocDiagramCmd, (d == 0) ? (void *) 0 : ((UmlBaseItem *) d)->_identifier);
    if (UmlCom::read_bool()) {
      _assoc_diagram = d;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  return the  return the isDimension attribute
 */
bool UmlBaseActivityPartition::isDimension() {
    read_if_needed_();
    return _dimension;
}

/**
 *  set the isDimension attribute
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBaseActivityPartition::set_isDimension(bool v) {
    return set_it_(_dimension, v, setMultiplicityCmd);
}

/**
 *  return the  return the isExternal attribute
 */
bool UmlBaseActivityPartition::isExternal() {
    read_if_needed_();
    return _external;
}

/**
 *  set the isExternal attribute
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBaseActivityPartition::set_isExternal(bool v) {
    return set_it_(_external, v, setIsCppExternalCmd);
}

/**
 *  return the represents
 */
UmlItem * UmlBaseActivityPartition::represents() {
    read_if_needed_();
    return _represents;
}

/**
 *  set the represents
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBaseActivityPartition::set_Represents(UmlItem * v) {
    UmlCom::send_cmd(_identifier, setDerivedCmd, (v == 0) ? (void *) v : ((UmlBaseItem *) v)->_identifier);
    if (UmlCom::read_bool()) {
      _represents = v;
      return TRUE;
    }
    else
      return FALSE;
}

void UmlBaseActivityPartition::read_uml_() {
    _assoc_diagram = (UmlActivityDiagram *) UmlBaseItem::read_();
    UmlBaseItem::read_uml_();
    _dimension = UmlCom::read_bool();
    _external = UmlCom::read_bool();
    _represents = (UmlItem *) UmlBaseItem::read_();
}


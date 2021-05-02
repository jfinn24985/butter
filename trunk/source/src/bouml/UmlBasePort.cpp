
#include "UmlCom.h"
#include "bouml/UmlBasePort.h"
#include "bouml/UmlPort.h"
#include "bouml/UmlClass.h"
#include "bouml/UmlState.h"

/**
 *   returns a new port named 's' created under 'parent'
 * 
 *  In case it cannot be created (the name is already used or
 *  invalid, 'parent' cannot contain it etc ...) return 0 in C++
 *  and produce a RuntimeException in Java
 */
UmlPort * UmlBasePort::create(UmlClass * parent, const char * s)
{
    return (UmlPort *) parent->create_(aPort, s);
}

/**
 *  returns the kind of the item
 */
anItemKind UmlBasePort::kind() {
    return aPort;
}

/**
 *  return if the port is read only
 */
bool UmlBasePort::isReadOnly() {
    read_if_needed_();
    return _isReadOnly;
}

/**
 * if the port is read only
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBasePort::set_isReadOnly(bool v) {
    UmlCom::send_cmd(_identifier, setReadOnlyCmd, (char) v);
    if (UmlCom::read_bool()) {
      _isReadOnly = v;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  return if the port is derived
 */
bool UmlBasePort::isDerived() {
    read_if_needed_();
    return _isDerived;
}

/**
 * if the port is derived
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBasePort::set_isDerived(bool v) {
    UmlCom::send_cmd(_identifier, setDerivedCmd, (char) v);
    if (UmlCom::read_bool()) {
      _isDerived = v;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  return if the port is a behavior port
 */
bool UmlBasePort::isBehavior() {
    read_if_needed_();
    return _isBehavior;
}

/**
 * if the port is a behavior port
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBasePort::set_isBehavior(bool v) {
    UmlCom::send_cmd(_identifier, setUmlEntryBehaviorCmd, (char) v);
    if (UmlCom::read_bool()) {
      _isBehavior = v;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  return if the port is Conjugated
 */
bool UmlBasePort::isConjugated() {
    read_if_needed_();
    return _isConjugated;
}

/**
 * if the port is Conjugated
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBasePort::set_isConjugated(bool v) {
    UmlCom::send_cmd(_identifier, setIsConjugatedCmd, (char) v);
    if (UmlCom::read_bool()) {
      _isConjugated = v;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  return if the port is used to provide the published functionality of an EncapsulatedClassifier
 */
bool UmlBasePort::isService() {
    read_if_needed_();
    return _isService;
}

/**
 * if the port is used to provide the published functionality of an EncapsulatedClassifier
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBasePort::set_isService(bool v) {
    UmlCom::send_cmd(_identifier, setIsServiceCmd, (char) v);
    if (UmlCom::read_bool()) {
      _isService = v;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  return the type
 */
UmlClass * UmlBasePort::type() {
    read_if_needed_();
    return _type;
}

/**
 *  set the type
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBasePort::set_Type(UmlClass * v) {
    UmlCom::send_cmd(_identifier, setTypeCmd, (v == 0) ? (void *) v : ((UmlBaseItem *) v)->_identifier);
    if (UmlCom::read_bool()) {
      _type = v;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  return the multiplicity
 */
const QByteArray & UmlBasePort::multiplicity() {
    read_if_needed_();
    return _multiplicity;
}

/**
 *  set the multiplicity
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBasePort::set_Multiplicity(const QByteArray v) {
    return set_it_(_multiplicity, v, setMultiplicityCmd);
}

/**
 *  return the protocol
 */
UmlState * UmlBasePort::protocol() {
    read_if_needed_();
    return _protocol;
}

/**
 *  set the protocol
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBasePort::set_Protocol(UmlState * v) {
    UmlCom::send_cmd(_identifier, setProtocolCmd, (v == 0) ? (void *) v : ((UmlBaseItem *) v)->_identifier);
    if (UmlCom::read_bool()) {
      _protocol = v;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  return the default value
 */
const QByteArray & UmlBasePort::defaultValue() {
    read_if_needed_();
    return _default_value;
}

/**
 *  set the default value
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBasePort::set_DefaultValue(const QByteArray v) {
    return set_it_(_default_value, v, setDefaultValueCmd);
}

const Q3PtrVector<UmlClass> & UmlBasePort::providedClasses() {
  read_if_needed_();
  
  return _provided;
}

const Q3PtrVector<UmlClass> & UmlBasePort::requiredClasses() {
  read_if_needed_();
  
  return _required;
}

/**
 *  set the provided and required classes lists
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBasePort::set_AssociatedClasses(const Q3PtrVector<UmlClass> & provided, const Q3PtrVector<UmlClass> & required) {
    Q3PtrVector<UmlClass> empty;
    
    UmlCom::send_cmd(_identifier, setAssocClassesCmd,
  		   empty, provided, required);
    if (UmlCom::read_bool()) {
      if (_defined) {
        _provided = provided;
        _required = required;
      }
      return TRUE;
    }
    else
      return FALSE;
}

void UmlBasePort::unload(bool rec, bool del) {
  _provided.clear();
  _required.clear();
  UmlBaseItem::unload(rec, del);
}

void UmlBasePort::read_uml_() {
  UmlBaseItem::read_uml_();
  _isReadOnly = UmlCom::read_bool();
  _isDerived = UmlCom::read_bool();
  _isBehavior = UmlCom::read_bool();
  _isConjugated = UmlCom::read_bool();
  _isService = UmlCom::read_bool();
  _type = (UmlClass *) UmlBaseItem::read_();
  _multiplicity = UmlCom::read_string();
  _protocol = (UmlState *) UmlBaseItem::read_();
  _default_value = UmlCom::read_string();
  
  unsigned n;
  unsigned index;
  
  n = UmlCom::read_unsigned();
  _provided.resize(n);
  
  for (index = 0; index != n; index += 1)
  _provided.insert(index, (UmlClass *) UmlBaseItem::read_());
  
  n = UmlCom::read_unsigned();
  _required.resize(n);
  
  for (index = 0; index != n; index += 1)
    _required.insert(index, (UmlClass *) UmlBaseItem::read_());
}


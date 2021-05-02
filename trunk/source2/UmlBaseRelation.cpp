
#include "UmlBaseRelation.h"
#include "UmlRelation.h"
#include "UmlClass.h"
#include "UmlOperation.h"
#include "UmlAttribute.h"

#include "UmlCom.h"
UmlRelation * UmlBaseRelation::create(aRelationKind kind, UmlClass * start, UmlClass * end)
{
  UmlCom::send_cmd(start->_identifier, createCmd, aRelation,
		   kind, end->_identifier);
  UmlRelation * result = (UmlRelation *) UmlBaseItem::read_();
  
  if (result != 0) {
    start->reread_children_if_needed_();
    
    switch (kind) {
    case anAssociation:
    case anAggregation:
    case anAggregationByValue:
      // bi-directional
      end->reread_children_if_needed_();
      break;
    case aGeneralisation:
    case aRealization:
      if (!end->formals().isEmpty())
	// to update actuals & base_type
	start->reread_if_needed_();
    default:
      break;
    }
  }
  
  return result;
}

anItemKind UmlBaseRelation::kind() {
  return aRelation;
}

aRelationKind UmlBaseRelation::relationKind() {
  read_if_needed_();
  
  return _rel_kind;
}

UmlRelation * UmlBaseRelation::side(bool first) {
  UmlCom::send_cmd(_identifier, sideCmd, (char) first);
  
  return (UmlRelation *) UmlBaseItem::read_();
}

// Set if the member is a 'class member' (static)
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseRelation::set_isClassMember(bool y) {
#ifdef WITHCPP
    if (!y)
      _cpp_thread_local = FALSE;
#endif
    return UmlBaseClassMember::set_isClassMember(y);
}

bool UmlBaseRelation::isReadOnly() {
  read_if_needed_();
  
  return _read_only;
}

// to set the 'read only' state of the relation
// 
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseRelation::set_isReadOnly(bool y) {
    UmlCom::send_cmd(_identifier, setIsReadOnlyCmd, (char) y);
    if (UmlCom::read_bool()) {
      _read_only = y;
      return TRUE;
    }
    else
      return FALSE;
}

// return the property 'derived'
bool UmlBaseRelation::isDerived() {
    read_if_needed_();
    return _derived;
}

// return the property 'derived union'
bool UmlBaseRelation::isDerivedUnion() {
    read_if_needed_();
    return _derived_union;
}

// Set the properties 'derived' and 'union'
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseRelation::set_isDerived(bool is_derived, bool is_union) {
    UmlCom::send_cmd(_identifier, setDerivedCmd,
                     (char) (((is_derived) ? 1 : 0) + ((is_union) ? 2 : 0)));
    if (UmlCom::read_bool()) {
      _derived = is_derived;
      _derived_union = is_union;
      return TRUE;
    }
    else
      return FALSE;
}

// return the property 'ordered'
bool UmlBaseRelation::isOrdered() {
    read_if_needed_();
    return _ordered;
}

// set the property 'ordered'
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseRelation::set_isOrdered(bool v) {
    UmlCom::send_cmd(_identifier, setOrderingCmd, (char) v);
    if (UmlCom::read_bool()) {
      _ordered = v;
      return TRUE;
    }
    else
      return FALSE;
}

// return the property 'unique'
bool UmlBaseRelation::isUnique() {
    read_if_needed_();
    return _unique;
}

// set the property 'unique'
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseRelation::set_isUnique(bool v) {
    UmlCom::send_cmd(_identifier, setUniqueCmd, (char) v);
    if (UmlCom::read_bool()) {
      _unique = v;
      return TRUE;
    }
    else
      return FALSE;
}

const QByteArray & UmlBaseRelation::defaultValue() {
  read_if_needed_();
  
  return _default_value;
}

bool UmlBaseRelation::set_DefaultValue(const char * s) {
  return set_it_(_default_value, s, setDefaultValueCmd);
}

bool UmlBaseRelation::set_Stereotype(const QByteArray & s) {
  if (UmlBaseItem::set_Stereotype(s)) {
    UmlBaseRelation * other = (UmlBaseRelation *) UmlBaseItem::read_();
    
    if (other != 0)
      other->_stereotype = s;
    return TRUE;
  }
  else
    return FALSE;
}

UmlClass * UmlBaseRelation::roleType() {
  read_if_needed_();
  
  return _role_type;
}

UmlTypeSpec UmlBaseRelation::association() {
  read_if_needed_();
  
  return _association;
}

bool UmlBaseRelation::set_Association(const UmlTypeSpec & t) {
  if (set_it_(_association, t, setRelationAssocClassCmd)) {
    UmlBaseRelation * other = (UmlBaseRelation *) UmlBaseItem::read_();
    
    if (other != 0)
      other->_association = t;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & UmlBaseRelation::roleName() {
  read_if_needed_();
  
  return _role_name;
}

bool UmlBaseRelation::set_RoleName(const char * s) {
  return set_it_(_role_name, s, setRoleNameCmd);
}

const QByteArray & UmlBaseRelation::multiplicity() {
  read_if_needed_();
  
  return _multiplicity;
}

bool UmlBaseRelation::set_Multiplicity(const char * s) {
  return set_it_(_multiplicity, s, setMultiplicityCmd);
}

UmlOperation * UmlBaseRelation::getOperation() {
  read_if_needed_();
  
  return _get_oper;
}

bool UmlBaseRelation::addGetOperation() {
  UmlCom::send_cmd(_identifier, addGetOperCmd);
  if (UmlCom::read_bool()) {
    reread_children_if_needed_();
    return TRUE;
  }
 else
   return FALSE;
}

UmlOperation * UmlBaseRelation::setOperation() {
  read_if_needed_();
  
  return _set_oper;
}

bool UmlBaseRelation::addSetOperation() {
  UmlCom::send_cmd(_identifier, addSetOperCmd);
  if (UmlCom::read_bool()) {
    reread_children_if_needed_();
    return TRUE;
  }
 else
   return FALSE;
}

#ifdef WITHCPP
bool UmlBaseRelation::cppVirtualInheritance() {
  read_if_needed_();
  
  return _cpp_virtual_inheritance;
}

bool UmlBaseRelation::set_CppVirtualInheritance(bool y) {
  bool b;
  
  if (set_it_(b, y, setCppVirtualInheritanceCmd)) {
    _cpp_virtual_inheritance = y;
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseRelation::isCppMutable() {
  read_if_needed_();
  
  return _cpp_mutable;
}

// Set if the relation is 'mutable'
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseRelation::set_isCppMutable(bool y) {
    UmlCom::send_cmd(_identifier, setIsCppMutableCmd, (char) y);
    if (UmlCom::read_bool()) {
      _cpp_mutable = y;
      return TRUE;
    }
    else
      return FALSE;
}
#endif

#ifdef WITHCPP
// Indicate if the relation is 'constexpr'
bool UmlBaseRelation::isCppConstExpr() {
    read_if_needed_();
  
    return _cpp_constexpr;
}

// Set if the relation is 'constexpr'
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseRelation::set_isCppConstExpr(bool y) {
    UmlCom::send_cmd(_identifier, setIsCppConstExprCmd, (char) y);
    if (UmlCom::read_bool()) {
      _cpp_constexpr = y;
      return TRUE;
    }
   else
     return FALSE;
}

// Indicate if the relation is 'thread_local'
bool UmlBaseRelation::isCppThreadLocal() {
    read_if_needed_();
  
    return _cpp_thread_local;
}

// Set if the relation is 'thread_local'
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseRelation::set_isCppThreadLocal(bool y) {
    UmlCom::send_cmd(_identifier, setIsCppThreadLocalCmd, (char) y);
    if (UmlCom::read_bool()) {
      _cpp_thread_local = y;
      if (y)
        _class_member = TRUE;
      return TRUE;
    }
   else
     return FALSE;
}
#endif

#ifdef WITHJAVA
bool UmlBaseRelation::isJavaTransient() {
  read_if_needed_();
  
  return _java_transient;
}

bool UmlBaseRelation::set_isJavaTransient(bool y) {
  bool b;
  
  if (set_it_(b, y, setIsJavaTransientCmd)) {
    _java_transient = y;
    return TRUE;
  }
  else
    return FALSE;

}
#endif

#ifdef WITHIDL
bool UmlBaseRelation::isIdlTruncatableInheritance() {
  read_if_needed_();
  
  return _idl_truncatable;
}

bool UmlBaseRelation::set_isIdlTruncatableInheritance(bool y) {
  bool b;
  
  if (set_it_(b, y, setIsIdlTruncatableCmd)) {
    _idl_truncatable = y;
    return TRUE;
  }
  else
    return FALSE;
}

QByteArray UmlBaseRelation::idlCase() {
  read_if_needed_();
  
  return (_idl_case != 0) ? _idl_case->name() : _idl_explicit_case;
}

bool UmlBaseRelation::set_IdlCase(UmlAttribute * a) {
  UmlCom::send_cmd(_identifier, setIdlCaseCmd, a->_identifier, "");
  if (UmlCom::read_bool()) {
    _idl_case = a;
    _idl_explicit_case = 0;
    return TRUE;
  }
 else
   return FALSE;
}

bool UmlBaseRelation::set_IdlCase(const char * s) {
  UmlCom::send_cmd(_identifier, setIdlCaseCmd, (void *) 0, s);
  if (UmlCom::read_bool()) {
    _idl_case = 0;
    _idl_explicit_case = s;
    return TRUE;
  }
 else
   return FALSE;
}
#endif

#ifdef WITHMYSQL
// in case the relation is a foreign key part of a table returns
// the constraint, else the return value doesn't have meaning.
const QByteArray & UmlBaseRelation::mysqlConstraint() {
    read_if_needed_();
  
    return _mysql_constraint;
}

// to set the constraint of the foreing key part of a table
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseRelation::set_MysqlConstraint(const char * s) {
    return set_it_(_mysql_constraint, s, setMysqlConstraintCmd);
}

// returns (in Java a copy of) the columns
// significant when the relation is a foreign key of a table
const Q3PtrVector<UmlAttribute> & UmlBaseRelation::mysqlColumns() {
    read_if_needed_();
  
    return _mysql_columns;
}

// to set the columns
// significant when the relation is a foreign key of a table
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseRelation::set_MysqlColumns(const Q3PtrVector<UmlAttribute> & l) {
    UmlCom::send_cmd(_identifier, setMysqlColumnsCmd, (const Q3PtrVector<UmlItem> &) l);
    if (UmlCom::read_bool()) {
      if (_defined)
        _mysql_columns = l;
      return TRUE;
    }
    else
      return FALSE;
}

// returns the key referenced by the foreing key or a null pointer
// significant when the relation is a foreign key of a table
UmlAttribute * UmlBaseRelation::mysqlReferencedKey() {
    read_if_needed_();
  
    return _mysql_ref_key;
}

// set the key referenced by the foreing key or a null pointer
// significant when the relation is a foreign key of a table
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseRelation::set_MysqlReferencedKey(UmlAttribute * k) {
    UmlCom::send_cmd(_identifier, setMysqlReferencedKeyCmd, (k != 0) ? ((UmlBaseItem *)k)->_identifier : 0);
    if (UmlCom::read_bool()) {
      _mysql_ref_key = k;
      return TRUE;
    }
    else
      return FALSE;
}

// returns what is done on delete
// significant when the relation is a foreign key of a table
aMySqlRefOption UmlBaseRelation::mysqlOnDelete() {
    read_if_needed_();
  
    return _mysql_on_delete;
}

// set what is done on delete
// significant when the relation is a foreign key of a table
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseRelation::set_MysqlOnDelete(aMySqlRefOption v) {
    UmlCom::send_cmd(_identifier, setMysqlOnDeleteCmd, (char) v);
    if (UmlCom::read_bool()) {
      _mysql_on_delete = v;
      return TRUE;
    }
    else
      return FALSE;
}

// returns what is done on update
// significant when the relation is a foreign key of a table
aMySqlRefOption UmlBaseRelation::mysqlOnUpdate() {
    read_if_needed_();
  
    return _mysql_on_update;
}

// set what is done on update
// significant when the relation is a foreign key of a table
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseRelation::set_MysqlOnUpdate(aMySqlRefOption v) {
    UmlCom::send_cmd(_identifier, setMysqlOnUpdateCmd, (char) v);
    if (UmlCom::read_bool()) {
      _mysql_on_update = v;
      return TRUE;
    }
    else
      return FALSE;
}
#endif

void UmlBaseRelation::unload(bool rec, bool del) {
  _role_name = 0;
  _multiplicity = 0;
  _default_value = 0;
#ifdef WITHMYSQL
  _mysql_constraint = 0;
  _mysql_columns.clear();
#endif
#ifdef WITHIDL
  _idl_explicit_case = 0;
#endif
  UmlBaseClassMember::unload(rec, del);
}

void UmlBaseRelation::read_uml_() {
  UmlBaseClassMember::read_uml_();
  _rel_kind = (aRelationKind) UmlCom::read_char();
  _role_type = (UmlClass *) UmlBaseItem::read_();
  _association.type = (UmlClass *) UmlBaseItem::read_();
  if (_association.type == 0)
    _association.explicit_type = UmlCom::read_string();
  _role_name = UmlCom::read_string();
  _multiplicity = UmlCom::read_string();
  _default_value = UmlCom::read_string();
  _read_only = UmlCom::read_bool();
  _derived = UmlCom::read_bool();
  _derived_union = UmlCom::read_bool();
  _ordered = UmlCom::read_bool();
  _unique = UmlCom::read_bool();
  _get_oper = (UmlOperation *) UmlBaseItem::read_();
  _set_oper = (UmlOperation *) UmlBaseItem::read_();
}

#ifdef WITHCPP
void UmlBaseRelation::read_cpp_() {
  UmlBaseClassMember::read_cpp_();
  _cpp_mutable = UmlCom::read_bool();
  _cpp_virtual_inheritance = UmlCom::read_bool();
  _cpp_constexpr = UmlCom::read_bool();
  _cpp_thread_local = UmlCom::read_bool();
}
#endif

#ifdef WITHJAVA
void UmlBaseRelation::read_java_() {
  UmlBaseClassMember::read_java_();
  _java_transient = UmlCom::read_bool();
}
#endif

#ifdef WITHPHP
void UmlBaseRelation::read_php_() {
  UmlBaseClassMember::read_php_();
}
#endif

#ifdef WITHPYTHON
//internal, do NOT use it

void UmlBaseRelation::read_python_() {
    UmlBaseClassMember::read_python_();
}
#endif

#ifdef WITHIDL
void UmlBaseRelation::read_idl_() {
  UmlBaseClassItem::read_idl_();
  _idl_truncatable = UmlCom::read_bool();
  _idl_case = (UmlAttribute *) UmlBaseItem::read_();
  if (_idl_case == 0)
    _idl_explicit_case = UmlCom::read_string();
}
#endif

#ifdef WITHMYSQL
//internal, do NOT use it

void UmlBaseRelation::read_mysql_() {
    UmlBaseClassItem::read_mysql_();
    
    _mysql_constraint = UmlCom::read_string();
    _mysql_on_delete = (aMySqlRefOption) UmlCom::read_char();
    _mysql_on_update = (aMySqlRefOption) UmlCom::read_char();
    _mysql_ref_key = (UmlAttribute *) UmlBaseItem::read_();
  
    unsigned n;
    unsigned index;
    
    n = UmlCom::read_unsigned();
    _mysql_columns.resize(n);
      
    for (index = 0; index != n; index += 1)
      _mysql_columns.insert(index, (UmlAttribute *) UmlBaseItem::read_());
}
#endif


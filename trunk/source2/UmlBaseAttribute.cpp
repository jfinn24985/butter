
#include "UmlBaseAttribute.h"
#include "UmlAttribute.h"
#include "UmlClass.h"
#include "UmlOperation.h"

#include "UmlCom.h"
UmlAttribute * UmlBaseAttribute::create(UmlClass * parent, const char * s)
{
  return (UmlAttribute *) parent->create_(anAttribute, s);
}

anItemKind UmlBaseAttribute::kind() {
  return anAttribute;
}

// Set if the member is a 'class member' (static)
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAttribute::set_isClassMember(bool y) {
#ifdef WITHCPP
    if (!y)
      _cpp_thread_local = FALSE;
#endif
    return UmlBaseClassMember::set_isClassMember(y);
}

bool UmlBaseAttribute::isReadOnly() {
  read_if_needed_();
  
  return _read_only;
}

// to set the 'read only' state of the attribute
// 
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAttribute::set_isReadOnly(bool y) {
    UmlCom::send_cmd(_identifier, setIsReadOnlyCmd, (char) y);
    if (UmlCom::read_bool()) {
      _read_only = y;
      return TRUE;
    }
    else
      return FALSE;
}

// return the property 'derived'
bool UmlBaseAttribute::isDerived() {
    read_if_needed_();
    return _derived;
}

// return the property 'derived union'
bool UmlBaseAttribute::isDerivedUnion() {
    read_if_needed_();
    return _derived_union;
}

// Set the properties 'derived' and 'union'
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAttribute::set_isDerived(bool is_derived, bool is_union) {
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
bool UmlBaseAttribute::isOrdered() {
    read_if_needed_();
    return _ordered;
}

// set the property 'ordered'
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAttribute::set_isOrdered(bool v) {
    UmlCom::send_cmd(_identifier, setOrderingCmd, (char) v);
    if (UmlCom::read_bool()) {
      _ordered = v;
      return TRUE;
    }
    else
      return FALSE;
}

// return the property 'unique'
bool UmlBaseAttribute::isUnique() {
    read_if_needed_();
    return _unique;
}

// set the property 'unique'
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAttribute::set_isUnique(bool v) {
    UmlCom::send_cmd(_identifier, setUniqueCmd, (char) v);
    if (UmlCom::read_bool()) {
      _unique = v;
      return TRUE;
    }
    else
      return FALSE;
}

const QByteArray & UmlBaseAttribute::defaultValue() {
  read_if_needed_();
  
  return _default_value;
}

bool UmlBaseAttribute::set_DefaultValue(const char * s) {
  return set_it_(_default_value, s, setDefaultValueCmd);
}

const UmlTypeSpec & UmlBaseAttribute::type() {
  read_if_needed_();
  
  return _type;
}

bool UmlBaseAttribute::set_Type(const UmlTypeSpec & t) {
  return set_it_(_type, t, setTypeCmd);
}

const QByteArray & UmlBaseAttribute::multiplicity() {
  read_if_needed_();
  
  return _multiplicity;
}

bool UmlBaseAttribute::set_Multiplicity(const char * s) {
  return set_it_(_multiplicity, s, setMultiplicityCmd);
}

UmlOperation * UmlBaseAttribute::getOperation() {
  read_if_needed_();
  
  return _get_oper;
}

bool UmlBaseAttribute::addGetOperation() {
  UmlCom::send_cmd(_identifier, addGetOperCmd);
  if (UmlCom::read_bool()) {
    reread_children_if_needed_();
    return TRUE;
  }
 else
   return FALSE;
}

UmlOperation * UmlBaseAttribute::setOperation() {
  read_if_needed_();
  
  return _set_oper;
}

bool UmlBaseAttribute::addSetOperation() {
  UmlCom::send_cmd(_identifier, addSetOperCmd);
  if (UmlCom::read_bool()) {
    reread_children_if_needed_();
    return TRUE;
  }
 else
   return FALSE;
}

#ifdef WITHCPP
bool UmlBaseAttribute::isCppMutable() {
  read_if_needed_();
  
  return _cpp_mutable;
}

// Set if the attribute is 'mutable'
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAttribute::set_isCppMutable(bool y) {
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
// Indicate if the attribute is 'constexpr'
bool UmlBaseAttribute::isCppConstExpr() {
    read_if_needed_();
  
    return _cpp_constexpr;
}

// Set if the attribute is 'constexpr'
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAttribute::set_isCppConstExpr(bool y) {
    UmlCom::send_cmd(_identifier, setIsCppConstExprCmd, (char) y);
    if (UmlCom::read_bool()) {
      _cpp_constexpr = y;
      return TRUE;
    }
   else
     return FALSE;
}

// Indicate if the attribute is 'thread_local'
bool UmlBaseAttribute::isCppThreadLocal() {
    read_if_needed_();
  
    return _cpp_thread_local;
}

// Set if the attribute is 'thread_local'
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAttribute::set_isCppThreadLocal(bool y) {
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
bool UmlBaseAttribute::isJavaTransient() {
  read_if_needed_();
  
  return _java_transient;
}

// to set the 'transient' state of the attribute
// 
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAttribute::set_isJavaTransient(bool y) {
    UmlCom::send_cmd(_identifier, setIsJavaTransientCmd, (char) y);
    if (UmlCom::read_bool()) {
      _java_transient = y;
      return TRUE;
    }
    else
      return FALSE;
}
#endif

#ifdef WITHIDL
QByteArray UmlBaseAttribute::idlCase() {
  read_if_needed_();
  
  return (_idl_case != 0) ? _idl_case->name() : _idl_explicit_case;
}

bool UmlBaseAttribute::set_IdlCase(UmlAttribute * a) {
  UmlCom::send_cmd(_identifier, setIdlCaseCmd, a->_identifier, "");
  if (UmlCom::read_bool()) {
    _idl_case = a;
    _idl_explicit_case = 0;
    return TRUE;
  }
 else
   return FALSE;
}

bool UmlBaseAttribute::set_IdlCase(const char * s) {
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
// in case the attribute is part of a table returns what this attribute
// represents, else the return value doesn't have meaning.
aMySqlKind UmlBaseAttribute::mysqlKind() {
    read_if_needed_();
  
    return _mysql_kind;
}

// to set the meaning of the attribute part of a table.
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAttribute::set_MysqlKind(aMySqlKind v) {
    UmlCom::send_cmd(_identifier, setMysqlKindCmd, (char) v);
    if (UmlCom::read_bool()) {
      if (_defined)
        _mysql_kind = v;
      return TRUE;
    }
    else
      return FALSE;
}

// in case the attribute is a columns part of a table indicates
// if it must be not null, else the return value doesn't have meaning
bool UmlBaseAttribute::isMysqlNotNull() {
    read_if_needed_();
  
    return _mysql_notnull_hash;
}

// to set if the columns part of a table must be not null
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAttribute::set_isMysqlNotNull(bool y) {
    UmlCom::send_cmd(_identifier, setIsMysqlNotNullHashCmd, (char) y);
    if (UmlCom::read_bool()) {
      _mysql_notnull_hash = y;
      return TRUE;
    }
   else
     return FALSE;
}

// in case the attribute is a columns part of a table indicates
// if auto increment is true, else the return value doesn't have meaning.
bool UmlBaseAttribute::isMysqlAutoIncrement() {
    read_if_needed_();
  
    return _mysql_autoincr_btree;
}

// to set if the columns part of a table must be in auto increment mode
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAttribute::set_isMysqlAutoIncrement(bool y) {
    UmlCom::send_cmd(_identifier, setIsMysqlAutoIncrBtreeCmd, (char) y);
    if (UmlCom::read_bool()) {
      _mysql_autoincr_btree = y;
      return TRUE;
    }
   else
     return FALSE;
}

// in case the attribute is a key part of a table returns the constraint,
// else the return value doesn't have meaning
const QByteArray & UmlBaseAttribute::mysqlConstraint() {
    read_if_needed_();
  
    return _mysql_constraint;
}

// to set the constraint of the key part of a table
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAttribute::set_MysqlConstraint(const char * s) {
    return set_it_(_mysql_constraint, s, setMysqlConstraintCmd);
}

// in case the attribute is a key part of a table indicates if it
// uses btree, else the return value doesn't have meaning.
bool UmlBaseAttribute::isMysqlUsingBtree() {
    read_if_needed_();
  
    return _mysql_autoincr_btree;
}

// to set if the key part of a table uses btree
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAttribute::set_isMysqlUsingBtree(bool y) {
    return set_isMysqlAutoIncrement(y);
}

// in case the attribute is a key part of a table indicates if it
// uses hash, else the return value doesn't have meaning.
bool UmlBaseAttribute::isMysqlUsingHash() {
    read_if_needed_();
  
    return _mysql_notnull_hash;
}

// to set if the key part of a table uses hash
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAttribute::set_isMysqlUsingHash(bool y) {
    return set_isMysqlNotNull(y);
}

// returns (in Java a copy of) the columns
// significant when the attribute is a key of a table
const Q3PtrVector<UmlAttribute> & UmlBaseAttribute::mysqlColumns() {
    read_if_needed_();
  
    return _mysql_columns;
}

// to set the columns
// significant when the attribute is a key of a table
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAttribute::set_MysqlColumns(const Q3PtrVector<UmlAttribute> & l) {
    UmlCom::send_cmd(_identifier, setMysqlColumnsCmd, (const Q3PtrVector<UmlItem> &) l);
    if (UmlCom::read_bool()) {
      if (_defined)
        _mysql_columns = l;
      return TRUE;
    }
    else
      return FALSE;
}
#endif

void UmlBaseAttribute::unload(bool rec, bool del) {
  _type.explicit_type = 0;
  _default_value = 0;
#ifdef WITHMYSQL
  _mysql_constraint = 0;
  _mysql_columns.clear();
#endif
#ifdef WITHIDL
  _idl_explicit_case = 0;
#endif
  UmlBaseClassMember::unload(rec, del);
  _multiplicity = 0;
}

void UmlBaseAttribute::read_uml_() {
  UmlBaseClassMember::read_uml_();
  _type.type = (UmlClass *) UmlBaseItem::read_();
  if (_type.type == 0)
    _type.explicit_type = UmlCom::read_string();
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
void UmlBaseAttribute::read_cpp_() {
  UmlBaseClassMember::read_cpp_();
  _cpp_mutable = UmlCom::read_bool();
  _cpp_constexpr = UmlCom::read_bool();
  _cpp_thread_local = UmlCom::read_bool();
}
#endif

#ifdef WITHJAVA
void UmlBaseAttribute::read_java_() {
  UmlBaseClassMember::read_java_();
  _java_transient = UmlCom::read_bool();
}
#endif

#ifdef WITHPHP
void UmlBaseAttribute::read_php_() {
  UmlBaseClassMember::read_php_();
}
#endif

#ifdef WITHPYTHON
//internal, do NOT use it

void UmlBaseAttribute::read_python_() {
    UmlBaseClassMember::read_python_();
}
#endif

#ifdef WITHIDL
void UmlBaseAttribute::read_idl_() {
  UmlBaseClassItem::read_idl_();
  _idl_case = (UmlAttribute *) UmlBaseItem::read_();
  if (_idl_case == 0)
    _idl_explicit_case = UmlCom::read_string();
}
#endif

#ifdef WITHMYSQL
//internal, do NOT use it

void UmlBaseAttribute::read_mysql_() {
    UmlBaseClassItem::read_mysql_();
    
    _mysql_constraint = UmlCom::read_string();
    _mysql_kind = (aMySqlKind) UmlCom::read_char();
    _mysql_autoincr_btree = UmlCom::read_bool();
    _mysql_notnull_hash = UmlCom::read_bool();
  
    unsigned n;
    unsigned index;
    
    n = UmlCom::read_unsigned();
    _mysql_columns.resize(n);
      
    for (index = 0; index != n; index += 1)
      _mysql_columns.insert(index, (UmlAttribute *) UmlBaseItem::read_());
}
#endif


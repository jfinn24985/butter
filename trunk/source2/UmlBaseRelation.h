#ifndef _UMLBASERELATION_H
#define _UMLBASERELATION_H


#include "UmlClassMember.h"
#include "aRelationKind.h"
#include "anItemKind.h"
#include <qbytearray.h>
#include "UmlTypeSpec.h"
#include <q3ptrvector.h>
#include "aMySqlRefOption.h"

class UmlRelation;
class UmlClass;
class UmlOperation;
class UmlAttribute;

// Manage the relations between classes
class UmlBaseRelation : public UmlClassMember {
  public:
    // returns a new relation of the given 'kind' from 'start' to 'end'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlRelation * create(aRelationKind kind, UmlClass * start, UmlClass * end);

    // returns the kind of the item
    virtual anItemKind kind();

    // returns the kind of the relation
    aRelationKind relationKind();

    // if 'first' is true returns the relation associated to the
    // first role, else the relation associated to the second
    // role or 0/null if the relation is uni directional
    UmlRelation * side(bool first);

    // Set if the member is a 'class member' (static)
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    virtual bool set_isClassMember(bool y);

    // indicates if the relation is read only, returns TRUE if yes
    bool isReadOnly();

    // to set the 'read only' state of the relation
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isReadOnly(bool y);

    // return the property 'derived'
    bool isDerived();

    // return the property 'derived union'
    bool isDerivedUnion();

    // Set the properties 'derived' and 'union'
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isDerived(bool is_derived, bool is_union);

    // return the property 'ordered'
    bool isOrdered();

    // set the property 'ordered'
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isOrdered(bool v);

    // return the property 'unique'
    bool isUnique();

    // set the property 'unique'
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isUnique(bool v);

    // returns the default relation value, may be an empty string
    const QByteArray & defaultValue();

    // to set the default relation value ("" allowed)
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_DefaultValue(const char * s);

    // to set the stereotype
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    // redefined in case the relation is bidirectional to set the stereotype
    // of the relation corresponding to the other direction
    virtual bool set_Stereotype(const QByteArray & s);

    // returns the 'end' class (the 'start' class is the parent of the relation) no set !
    UmlClass * roleType();

    // return the associated class/type, may be an empty spec
    UmlTypeSpec association();

    // set the associated class/type, may be an empty spec
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    bool set_Association(const UmlTypeSpec & t);

    // returns the name of the role
    const QByteArray & roleName();

    // to set the name of the role
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_RoleName(const char * s);

    // returns the multiplicity (may be an empty string)
    const QByteArray & multiplicity();

    // to set the multiplicity
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Multiplicity(const char * s);

    // returns the 'get' operation of the relation, or 0 if it does not exist
    UmlOperation * getOperation();

    // to generate an associated 'get' operation
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool addGetOperation();

    // returns the 'set' operation of the relation, or 0 if it does not exist
    UmlOperation * setOperation();

    // to generate an associated 'set' operation
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool addSetOperation();

#ifdef WITHCPP
    // indicates if the inheritance is virtual in C++, returns TRUE if yes
    bool cppVirtualInheritance();

    // to set if the inheritance is virtual in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppVirtualInheritance(bool y);

    // Indicate if the relation is 'mutable'
    bool isCppMutable();

    // Set if the relation is 'mutable'
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isCppMutable(bool y);
#endif

#ifdef WITHCPP
    // Indicate if the relation is 'constexpr'
    bool isCppConstExpr();

    // Set if the relation is 'constexpr'
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    bool set_isCppConstExpr(bool y);

    // Indicate if the relation is 'thread_local'
    bool isCppThreadLocal();

    // Set if the relation is 'thread_local'
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    bool set_isCppThreadLocal(bool y);
#endif

#ifdef WITHJAVA
    // indicates if the relation is 'transient', returns TRUE if yes
    bool isJavaTransient();

    // to set the 'transient' state of the relation
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isJavaTransient(bool y);
#endif

#ifdef WITHIDL
    // indicates if the relation is 'truncatable', returns TRUE if yes
    // have sense only for a valuetype inheritance
    bool isIdlTruncatableInheritance();

    // to set if the inheritance is 'truncatable'
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isIdlTruncatableInheritance(bool y);

    // in case the relation is an IDL union's member returns the
    // corresponding 'case', an empty string in case it is not specified
    QByteArray idlCase();

    // to set the 'case' even the relation is not (already) known as
    // an IDL union's member
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_IdlCase(UmlAttribute * a);

    // to set the 'case' even the relation is not (already) known as
    // an IDL union's member
    // 
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_IdlCase(const char * s);
#endif

#ifdef WITHMYSQL
    // in case the relation is a foreign key part of a table returns
    // the constraint, else the return value doesn't have meaning.
    const QByteArray & mysqlConstraint();

    // to set the constraint of the foreing key part of a table
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    bool set_MysqlConstraint(const char * s);

    // returns (in Java a copy of) the columns
    // significant when the relation is a foreign key of a table
    const Q3PtrVector<UmlAttribute> & mysqlColumns();

    // to set the columns
    // significant when the relation is a foreign key of a table
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    bool set_MysqlColumns(const Q3PtrVector<UmlAttribute> & l);

    // returns the key referenced by the foreing key or a null pointer
    // significant when the relation is a foreign key of a table
    UmlAttribute * mysqlReferencedKey();

    // set the key referenced by the foreing key or a null pointer
    // significant when the relation is a foreign key of a table
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    bool set_MysqlReferencedKey(UmlAttribute * k);

    // returns what is done on delete
    // significant when the relation is a foreign key of a table
    aMySqlRefOption mysqlOnDelete();

    // set what is done on delete
    // significant when the relation is a foreign key of a table
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    bool set_MysqlOnDelete(aMySqlRefOption v);

    // returns what is done on update
    // significant when the relation is a foreign key of a table
    aMySqlRefOption mysqlOnUpdate();

    // set what is done on update
    // significant when the relation is a foreign key of a table
    //
    // On error : return FALSE in C++, produce a RuntimeException in Java
    bool set_MysqlOnUpdate(aMySqlRefOption v);
#endif

    // to unload the object to free memory, it will be reloaded
    // automatically if needed. args unused
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    aRelationKind _rel_kind : 8;

#ifdef WITHMYSQL
    aMySqlRefOption _mysql_on_delete : 8;

    aMySqlRefOption _mysql_on_update : 8;
#endif

    bool _read_only : 1;

    bool _derived : 1;

    bool _derived_union : 1;

    bool _ordered : 1;

    bool _unique : 1;

#ifdef WITHCPP
    bool _cpp_virtual_inheritance : 1;

    bool _cpp_mutable : 1;
#endif

#ifdef WITHCPP
    bool _cpp_constexpr : 1;

    bool _cpp_thread_local : 1;
#endif

#ifdef WITHJAVA
    bool _java_transient : 1;
#endif

#ifdef WITHIDL
    bool _idl_truncatable : 1;
#endif

    QByteArray _default_value;

    UmlClass * _role_type;

    QByteArray _role_name;

    QByteArray _multiplicity;

    UmlTypeSpec _association;

    UmlOperation * _get_oper;

    UmlOperation * _set_oper;

    // exclusive with idl_explicit_case
    UmlAttribute * _idl_case;

#ifdef WITHIDL
    QByteArray _idl_explicit_case;
#endif

#ifdef WITHMYSQL
    QByteArray _mysql_constraint;

    UmlAttribute * _mysql_ref_key;

    Q3PtrVector<UmlAttribute> _mysql_columns;
#endif


  protected:
    //internal, do NOT use it
    
    virtual void read_uml_();

#ifdef WITHCPP
    //internal, do NOT use it
    
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    //internal, do NOT use it
    
    virtual void read_java_();
#endif

#ifdef WITHPHP
    //internal, do NOT use it
    
    virtual void read_php_();
#endif

#ifdef WITHPYTHON
    //internal, do NOT use it
    
    virtual void read_python_();
#endif

#ifdef WITHIDL
    //internal, do NOT use it
    
    virtual void read_idl_();
#endif

#ifdef WITHMYSQL
    //internal, do NOT use it
    
    virtual void read_mysql_();
#endif

    // the constructor, do not call it yourself !!!!!!!!!!
    UmlBaseRelation(void * id, const QByteArray & n);

};

inline UmlBaseRelation::UmlBaseRelation(void * id, const QByteArray & n) : UmlClassMember(id, n) {
  _role_type = 0;
  _get_oper = 0;
  _set_oper = 0;
#ifdef WITHMYSQL
  _mysql_ref_key = 0;
#endif
#ifdef WITHIDL
  _idl_case = 0;
#endif
}

#endif

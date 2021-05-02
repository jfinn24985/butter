
#include "UmlCom.h"
#include "UmlBaseExtraArtifactDefinition.h"
#include "UmlExtraArtifactDefinition.h"
#include "UmlArtifact.h"

//  returns a new extra artifact definition named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlExtraArtifactDefinition * UmlBaseExtraArtifactDefinition::create(UmlArtifact * parent, const char * s)
{
    return (UmlExtraArtifactDefinition *) parent->create_(anExtraArtifactDefinition, s);
}

// returns the kind of the item
anItemKind UmlBaseExtraArtifactDefinition::kind() {
    return anExtraArtifactDefinition;
}

#ifdef WITHCPP
// return the C++ declaration
const QByteArray & UmlBaseExtraArtifactDefinition::cppDecl() {
    read_if_needed_();
    return _cpp_decl;
}

// set the C++ declaration
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseExtraArtifactDefinition::set_CppDecl(const char * v) {
    return set_it_(_cpp_decl, v, setCppDeclCmd);
}

// return the C++ definition
const QByteArray & UmlBaseExtraArtifactDefinition::cppDef() {
    read_if_needed_();
    return _cpp_def;
}

// set the C++ definition
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseExtraArtifactDefinition::set_CppDef(const char * v) {
    return set_it_(_cpp_def, v, setCppDefCmd);
}
#endif

#ifdef WITHJAVA
// return the Java declaration
const QByteArray & UmlBaseExtraArtifactDefinition::javaDecl() {
    read_if_needed_();
    return _java_decl;
}

// set the Java declaration
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseExtraArtifactDefinition::set_JavaDecl(const char * v) {
    return set_it_(_java_decl, v, setJavaDeclCmd);
}
#endif

#ifdef WITHPHP
// return the Php declaration
const QByteArray & UmlBaseExtraArtifactDefinition::phpDecl() {
    read_if_needed_();
    return _php_decl;
}

// set the Php declaration
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseExtraArtifactDefinition::set_PhpDecl(const char * v) {
    return set_it_(_php_decl, v, setPhpDeclCmd);
}
#endif

#ifdef WITHPYTHON
// return the Python declaration
const QByteArray & UmlBaseExtraArtifactDefinition::pythonDecl() {
    read_if_needed_();
    return _python_decl;
}

// set the Python declaration
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseExtraArtifactDefinition::set_PythonDecl(const char * v) {
    return set_it_(_python_decl, v, setPhpDeclCmd);
}
#endif

#ifdef WITHIDL
// return the Idl declaration
const QByteArray & UmlBaseExtraArtifactDefinition::idlDecl() {
    read_if_needed_();
    return _idl_decl;
}

// set the Idl declaration
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseExtraArtifactDefinition::set_IdlDecl(const char * v) {
    return set_it_(_idl_decl, v, setPhpDeclCmd);
}
#endif

#ifdef WITHMYSQL
// return the Mysql declaration
const QByteArray & UmlBaseExtraArtifactDefinition::mysqlDecl() {
    read_if_needed_();
    return _mysql_decl;
}

// set the Mysql declaration
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseExtraArtifactDefinition::set_MysqlDecl(const char * v) {
    return set_it_(_mysql_decl, v, setPhpDeclCmd);
}
#endif

void UmlBaseExtraArtifactDefinition::unload(bool rec, bool del) {
#ifdef WITHCPP
  _cpp_def = 0;
  _cpp_decl = 0;
#endif
#ifdef WITHJAVA
  _java_decl = 0;
#endif
#ifdef WITHPHP
  _php_decl = 0;
#endif
#ifdef WITHPYTHON
  _python_decl = 0;
#endif
#ifdef WITHIDL
  _idl_decl = 0;
#endif
#ifdef WITHMYSQL
  _mysql_decl = 0;
#endif
  UmlBaseItem::unload(rec, del);
}

#ifdef WITHCPP
void UmlBaseExtraArtifactDefinition::read_cpp_() {
    _cpp_decl = UmlCom::read_string();
    _cpp_def = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseExtraArtifactDefinition::read_java_() {
  _java_decl = UmlCom::read_string();
}
#endif

#ifdef WITHPHP
void UmlBaseExtraArtifactDefinition::read_php_() {
  _php_decl = UmlCom::read_string();
}
#endif

#ifdef WITHPYTHON
//internal, do NOT use it

void UmlBaseExtraArtifactDefinition::read_python_() {
    _python_decl = UmlCom::read_string();
}
#endif

#ifdef WITHIDL
void UmlBaseExtraArtifactDefinition::read_idl_() {
  _idl_decl = UmlCom::read_string();
}
#endif

#ifdef WITHMYSQL
//internal, do NOT use it

void UmlBaseExtraArtifactDefinition::read_mysql_() {
    _mysql_decl = UmlCom::read_string();
}
#endif


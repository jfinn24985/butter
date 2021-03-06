
#include "UmlBaseClassItem.h"

#include "UmlCom.h"
#ifdef WITHCPP
const QByteArray & UmlBaseClassItem::cppDecl() {
  read_if_needed_();
  
  return _cpp_decl;
}

bool UmlBaseClassItem::set_CppDecl(const char * s) {
  return set_it_(_cpp_decl, s, setCppDeclCmd);
}
#endif

#ifdef WITHJAVA
const QByteArray & UmlBaseClassItem::javaDecl() {
  read_if_needed_();
  
  return _java_decl;
}

bool UmlBaseClassItem::set_JavaDecl(const char * s) {
  return set_it_(_java_decl, s, setJavaDeclCmd);
}
#endif

#ifdef WITHPHP
const QByteArray & UmlBaseClassItem::phpDecl() {
  read_if_needed_();
  
  return _php_decl;
}

bool UmlBaseClassItem::set_PhpDecl(const char * s) {
  return set_it_(_php_decl, s, setPhpDeclCmd);
}
#endif

#ifdef WITHPYTHON
const QByteArray & UmlBaseClassItem::pythonDecl() {
  read_if_needed_();
  
  return _python_decl;
}

bool UmlBaseClassItem::set_PythonDecl(const char * s) {
  return set_it_(_python_decl, s, setPythonDeclCmd);
}
#endif

#ifdef WITHIDL
const QByteArray & UmlBaseClassItem::idlDecl() {
  read_if_needed_();
  
  return _idl_decl;
}

bool UmlBaseClassItem::set_IdlDecl(const char * s) {
  return set_it_(_idl_decl, s, setIdlDeclCmd);
}
#endif

#ifdef WITHMYSQL
// return the MYSQL declaration
const QByteArray & UmlBaseClassItem::mysqlDecl() {
    read_if_needed_();
  
    return _mysql_decl;
}

// set the MYSQL definition
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseClassItem::set_MysqlDecl(const char * s) {
    return set_it_(_mysql_decl, s, setMysqlDeclCmd);
}
#endif

void UmlBaseClassItem::unload(bool rec, bool del) {
#ifdef WITHCPP
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
#ifdef WITHMYSQL
  _mysql_decl = 0;
#endif
#ifdef WITHIDL
  _idl_decl = 0;
#endif
  UmlBaseItem::unload(rec, del);
}

#ifdef WITHCPP
void UmlBaseClassItem::read_cpp_() {
  _cpp_decl = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseClassItem::read_java_() {
  _java_decl = UmlCom::read_string();
}
#endif

#ifdef WITHPHP
void UmlBaseClassItem::read_php_() {
  _php_decl = UmlCom::read_string();
}
#endif

#ifdef WITHPYTHON
//internal, do NOT use it

void UmlBaseClassItem::read_python_() {
    _python_decl = UmlCom::read_string();
}
#endif

#ifdef WITHIDL
void UmlBaseClassItem::read_idl_() {
  _idl_decl = UmlCom::read_string();
}
#endif

#ifdef WITHMYSQL
//internal, do NOT use it

void UmlBaseClassItem::read_mysql_() {
    _mysql_decl = UmlCom::read_string();
}
#endif


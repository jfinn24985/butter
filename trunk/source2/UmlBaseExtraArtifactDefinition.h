#ifndef _UMLBASEEXTRAARTIFACTDEFINITION_H
#define _UMLBASEEXTRAARTIFACTDEFINITION_H


#include "UmlItem.h"
#include "anItemKind.h"
#include <qbytearray.h>

class UmlExtraArtifactDefinition;
class UmlArtifact;

class UmlBaseExtraArtifactDefinition : public UmlItem {
  public:
    //  returns a new extra artifact definition named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlExtraArtifactDefinition * create(UmlArtifact * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();

#ifdef WITHCPP
    // return the C++ declaration
    const QByteArray & cppDecl();

    // set the C++ declaration
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppDecl(const char * v);

    // return the C++ definition
    const QByteArray & cppDef();

    // set the C++ definition
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppDef(const char * v);
#endif

#ifdef WITHJAVA
    // return the Java declaration
    const QByteArray & javaDecl();

    // set the Java declaration
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaDecl(const char * v);
#endif

#ifdef WITHPHP
    // return the Php declaration
    const QByteArray & phpDecl();

    // set the Php declaration
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_PhpDecl(const char * v);
#endif

#ifdef WITHPYTHON
    // return the Python declaration
    const QByteArray & pythonDecl();

    // set the Python declaration
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_PythonDecl(const char * v);
#endif

#ifdef WITHIDL
    // return the Idl declaration
    const QByteArray & idlDecl();

    // set the Idl declaration
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_IdlDecl(const char * v);
#endif

#ifdef WITHMYSQL
    // return the Mysql declaration
    const QByteArray & mysqlDecl();

    // set the Mysql declaration
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_MysqlDecl(const char * v);
#endif

    // to unload the object to free memory, it will be reloaded
    // automatically if needed. args unused
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
#ifdef WITHCPP
    QByteArray _cpp_decl;

    QByteArray _cpp_def;
#endif

#ifdef WITHJAVA
    QByteArray _java_decl;
#endif

#ifdef WITHPHP
    QByteArray _php_decl;
#endif

#ifdef WITHPYTHON
    QByteArray _python_decl;
#endif

#ifdef WITHIDL
    QByteArray _idl_decl;
#endif

#ifdef WITHMYSQL
    QByteArray _mysql_decl;
#endif


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseExtraArtifactDefinition(void * id, const QByteArray & s) : UmlItem(id, s) {
    }

#ifdef WITHCPP
    void read_cpp_();
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

};
#endif

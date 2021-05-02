#ifndef _UMLBASEPORTREF_H
#define _UMLBASEPORTREF_H


#include "UmlConnectable.h"
#include "anItemKind.h"
#include <qbytearray.h>

class UmlPortRef;
class UmlPortRefOwner;
class UmlPort;

class UmlBasePortRef : public UmlConnectable {
  public:
    //  returns a new port reference named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlPortRef * create(UmlPortRefOwner * parent, const char * s, UmlPort * port);

    // returns the kind of the item
    virtual anItemKind kind();

    // return the port
    UmlPort * port();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBasePortRef(void * id, const QByteArray & s) : UmlConnectable(id, s) {
    }

    //internal, do NOT use it
    
    virtual void read_uml_();


  private:
    UmlPort * _port;

};
#endif

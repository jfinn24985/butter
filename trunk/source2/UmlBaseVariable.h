#ifndef _UMLBASEVARIABLE_H
#define _UMLBASEVARIABLE_H


#include "UmlItem.h"
#include "anItemKind.h"
#include "UmlTypeSpec.h"
#include <qbytearray.h>
#include "aVisibility.h"

class UmlVariable;

class UmlBaseVariable : public UmlItem {
  public:
    //  returns a new variable named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlVariable * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();

    // return the type
    const UmlTypeSpec & type();

    // set the type
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Type(const UmlTypeSpec v);

    // return the multiplicity
    const QByteArray & multiplicity();

    // set the multiplicity
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Multiplicity(const QByteArray v);

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

    // return the visibility
    aVisibility visibility();

    // set the visibility
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Visibility(aVisibility v);


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseVariable(void * id, const QByteArray & s) : UmlItem(id, s) {
    }

    void read_uml_();


  private:
    UmlTypeSpec _type;

    QByteArray _multiplicity;

    bool _ordered;

    bool _unique;

    aVisibility _visibility;

};
#endif

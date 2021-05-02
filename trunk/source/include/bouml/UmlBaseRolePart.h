#ifndef _UMLBASEROLEPART_H
#define _UMLBASEROLEPART_H


#include "bouml/UmlPortRefOwner.h"
#include "bouml/anItemKind.h"
#include <qbytearray.h>

class UmlRolePart;
class UmlClass;

class UmlBaseRolePart : public UmlPortRefOwner {
  public:
    /**
     *   returns a new role/part named 's' created under 'parent'
     * 
     *  In case it cannot be created (the name is already used or
     *  invalid, 'parent' cannot contain it etc ...) return 0 in C++
     *  and produce a RuntimeException in Java
     */
    static UmlRolePart * create(UmlClass * parent, const char * s);

    /**
     *  returns the kind of the item
     */
    virtual anItemKind kind();

    /**
     *  return the element is a part if true, else a role
     */
    bool isPart();

    /**
     *  set set if the element is a part or a role
     * 
     *  On error return FALSE in C++, produce a RuntimeException in Java
     */
    bool set_isPart(bool v);

    /**
     *  return the type
     */
    UmlClass * type();

    /**
     *  set the type
     * 
     *  On error return FALSE in C++, produce a RuntimeException in Java
     */
    bool set_Type(UmlClass * v);

    /**
     *  return the multiplicity
     */
    const QByteArray & multiplicity();

    /**
     *  set the multiplicity
     * 
     *  On error return FALSE in C++, produce a RuntimeException in Java
     */
    bool set_Multiplicity(const QByteArray v);


  protected:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlBaseRolePart(void * id, const QByteArray & s) : UmlPortRefOwner(id, s) {
    }

    /**
     * internal, do NOT use it
     */
    virtual void read_uml_();


  private:
    bool _isPart;

    UmlClass * _type;

    QByteArray _multiplicity;

};
#endif

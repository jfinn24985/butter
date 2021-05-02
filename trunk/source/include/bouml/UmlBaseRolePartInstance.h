#ifndef _UMLBASEROLEPARTINSTANCE_H
#define _UMLBASEROLEPARTINSTANCE_H


#include "SlotAttribute.h"
#include "bouml/UmlPortRefOwner.h"
#include "bouml/anItemKind.h"
#include <q3valuelist.h>
#include <qbytearray.h>

class UmlRolePartInstance;
class UmlClassInstance;
class UmlRolePart;
struct SlotAttribute;
class UmlAttribute;

class UmlBaseRolePartInstance : public UmlPortRefOwner {
  public:
    /**
     *   returns a new role/part instance named 's' created under 'parent'
     * 
     *  In case it cannot be created (the name is already used or
     *  invalid, 'parent' cannot contain it etc ...) return 0 in C++
     *  and produce a RuntimeException in Java
     */
    static UmlRolePartInstance * create(UmlClassInstance * parent, const char * s, UmlRolePart * type);

    /**
     *  returns the kind of the item
     */
    virtual anItemKind kind();

    /**
     *  return the role/part typing the instance
     */
    UmlRolePart * type();

    /**
     *  Returns the attributes having a value
     */
    void attributesValue(Q3ValueList<SlotAttribute> & result);

    /**
     *  Remove the slot if the value is null.
     *  Else set the value for the given attribute, replacing it
     *  if the slot already exist.
     *  On error : return FALSE in C++, produce a RuntimeException in Java
     */
    bool set_AttributeValue(UmlAttribute * attribute, const char * value);


  protected:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlBaseRolePartInstance(void * id, const QByteArray & s) : UmlPortRefOwner(id, s) {
    }

    /**
     * internal, do NOT use it
     */
    virtual void read_uml_();


  private:
    UmlRolePart * _type;

};
#endif

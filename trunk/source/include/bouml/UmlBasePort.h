#ifndef _UMLBASEPORT_H
#define _UMLBASEPORT_H


#include "bouml/UmlItem.h"
#include "bouml/anItemKind.h"
#include <qbytearray.h>
#include <q3ptrvector.h>

class UmlPort;
class UmlClass;
class UmlState;

class UmlBasePort : public UmlItem {
  public:
    /**
     *   returns a new port named 's' created under 'parent'
     * 
     *  In case it cannot be created (the name is already used or
     *  invalid, 'parent' cannot contain it etc ...) return 0 in C++
     *  and produce a RuntimeException in Java
     */
    static UmlPort * create(UmlClass * parent, const char * s);

    /**
     *  returns the kind of the item
     */
    virtual anItemKind kind();

    /**
     *  return if the port is read only
     */
    bool isReadOnly();

    /**
     * if the port is read only
     * 
     *  On error return FALSE in C++, produce a RuntimeException in Java
     */
    bool set_isReadOnly(bool v);

    /**
     *  return if the port is derived
     */
    bool isDerived();

    /**
     * if the port is derived
     * 
     *  On error return FALSE in C++, produce a RuntimeException in Java
     */
    bool set_isDerived(bool v);

    /**
     *  return if the port is a behavior port
     */
    bool isBehavior();

    /**
     * if the port is a behavior port
     * 
     *  On error return FALSE in C++, produce a RuntimeException in Java
     */
    bool set_isBehavior(bool v);

    /**
     *  return if the port is Conjugated
     */
    bool isConjugated();

    /**
     * if the port is Conjugated
     * 
     *  On error return FALSE in C++, produce a RuntimeException in Java
     */
    bool set_isConjugated(bool v);

    /**
     *  return if the port is used to provide the published functionality of an EncapsulatedClassifier
     */
    bool isService();

    /**
     * if the port is used to provide the published functionality of an EncapsulatedClassifier
     * 
     *  On error return FALSE in C++, produce a RuntimeException in Java
     */
    bool set_isService(bool v);

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

    /**
     *  return the protocol
     */
    UmlState * protocol();

    /**
     *  set the protocol
     * 
     *  On error return FALSE in C++, produce a RuntimeException in Java
     */
    bool set_Protocol(UmlState * v);

    /**
     *  return the default value
     */
    const QByteArray & defaultValue();

    /**
     *  set the default value
     * 
     *  On error return FALSE in C++, produce a RuntimeException in Java
     */
    bool set_DefaultValue(const QByteArray v);

    /**
     *  returns (in Java a copy of) the optional provided classes
     */
    const Q3PtrVector<UmlClass> & providedClasses();

    /**
     *  returns (in Java a copy of) the optional required classes
     */
    const Q3PtrVector<UmlClass> & requiredClasses();

    /**
     *  set the provided and required classes lists
     * 
     *  On error return FALSE in C++, produce a RuntimeException in Java
     */
    bool set_AssociatedClasses(const Q3PtrVector<UmlClass> & provided, const Q3PtrVector<UmlClass> & required);

    /**
     *  to unload the object to free memory, it will be reloaded automatically
     *  if needed. Recursively done for the sub items if 'rec' is TRUE. 
     * 
     *  if 'del' is true the sub items are deleted in C++, and removed from the
     *  internal dictionnary in C++ and Java (to allow it to be garbaged),
     *  you will have to call Children() to re-access to them
     */
    virtual void unload(bool rec = FALSE, bool del = FALSE);


  protected:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlBasePort(void * id, const QByteArray & s) : UmlItem(id, s) {
    }

    /**
     * internal, do NOT use it
     */
    virtual void read_uml_();


  private:
    bool _isReadOnly:1;

    bool _isDerived:1;

    bool _isBehavior:1;

    bool _isConjugated:1;

    bool _isService:1;

    UmlClass * _type;

    QByteArray _multiplicity;

    UmlState * _protocol;

    QByteArray _default_value;

    Q3PtrVector<UmlClass> _provided;

    Q3PtrVector<UmlClass> _required;

};
#endif

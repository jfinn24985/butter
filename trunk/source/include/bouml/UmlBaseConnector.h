#ifndef _UMLBASECONNECTOR_H
#define _UMLBASECONNECTOR_H


#include "bouml/UmlItem.h"
#include "bouml/anItemKind.h"
#include <qbytearray.h>

class UmlConnector;
class UmlConnectable;

class UmlBaseConnector : public UmlItem {
  public:
    /**
     *  returns a new connector between 'c1' and 'c2'
     * 
     *  In case it cannot be created return 0 in C++ and produce a RuntimeException in Java
     */
    static UmlConnector * create(UmlConnectable * c1, UmlConnectable * c2);

    /**
     *  returns the kind of the item
     */
    virtual anItemKind kind();

    /**
     *  to set the name
     *  
     *  On error return FALSE in C++, produce a RuntimeException in Java
     */
    virtual bool set_Name(const QByteArray & s);

    /**
     *  to set the stereotype
     *   On error return FALSE in C++, produce a RuntimeException in Java
     */
    virtual bool set_Stereotype(const QByteArray & s);

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
     *  return the target of the connector, see also use side()
     */
    UmlConnectable * connectable();

    /**
     *  if 'first' is true returns the first side of the connector, else the second
     */
    UmlConnector * side(bool first);


  protected:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlBaseConnector(void * id, const QByteArray & s) : UmlItem(id, s) {
    }

    /**
     * internal, do NOT use it
     */
    virtual void read_uml_();


  private:
    QByteArray _multiplicity;

    UmlConnectable * _connectable;

};
#endif

#ifndef _UMLBASEACCESSVARIABLEVALUEACTION_H
#define _UMLBASEACCESSVARIABLEVALUEACTION_H


#include "UmlActivityAction.h"
#include <qbytearray.h>

class UmlVariable;

class UmlBaseAccessVariableValueAction : public UmlActivityAction {
  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseAccessVariableValueAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the variable
    UmlVariable * variable();

    // set the variable
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Variable(UmlVariable * v);


  protected:
    virtual void read_uml_();


  private:
    UmlVariable * _variable;

};

#endif

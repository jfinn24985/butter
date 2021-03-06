#ifndef _UMLBASECHOICEPSEUDOSTATE_H
#define _UMLBASECHOICEPSEUDOSTATE_H


#include "bouml/UmlPseudoState.h"
#include "bouml/anItemKind.h"
#include <qbytearray.h>

class UmlChoicePseudoState;
class UmlItem;

class UmlBaseChoicePseudoState : public UmlPseudoState {
  public:
    /**
     *  returns a new choice pseudo state created under 'parent'
     * 
     *  In case it cannot be created ('parent' cannot contain it etc ...)
     *  return 0 in C++ and produce a RuntimeException in Java
     */
    static UmlChoicePseudoState * create(UmlItem * parent);

    /**
     *  returns the kind of the item
     */
    virtual anItemKind kind();


  protected:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlBaseChoicePseudoState(void * id, const QByteArray & s) : UmlPseudoState(id, s) {
    }

};

#endif

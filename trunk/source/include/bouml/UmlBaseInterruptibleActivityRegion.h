#ifndef _UMLBASEINTERRUPTIBLEACTIVITYREGION_H
#define _UMLBASEINTERRUPTIBLEACTIVITYREGION_H


#include "bouml/UmlActivityRegion.h"
#include "bouml/anItemKind.h"
#include <qbytearray.h>

class UmlInterruptibleActivityRegion;
class UmlItem;

class UmlBaseInterruptibleActivityRegion : public UmlActivityRegion {
  public:
    /**
     *   returns a new interruptible activity region named 's' created under 'parent'
     * 
     *  In case it cannot be created (the name is already used or
     *  invalid, 'parent' cannot contain it etc ...) return 0 in C++
     *  and produce a RuntimeException in Java
     */
    static UmlInterruptibleActivityRegion * create(UmlItem * parent, const char * s);

    /**
     *  returns the kind of the item
     */
    virtual anItemKind kind();


  protected:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlBaseInterruptibleActivityRegion(void * id, const QByteArray & s) : UmlActivityRegion(id, s) {
    }

};

#endif

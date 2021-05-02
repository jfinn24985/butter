#ifndef _UMLITEM_H
#define _UMLITEM_H


#include "UmlBaseItem.h"
#include <qbytearray.h>

// This class is a mother class for all the other Uml* classes, this
// allows to generalize their management, declare virtual operations etc ...
//
// You can modify it as you want (except the constructor)
class UmlItem : public UmlBaseItem {
  public:
    UmlItem(void * id, const QByteArray & n) : UmlBaseItem(id, n) {};

    virtual ~UmlItem();

    //Attempt to get butter relevant property values from the object properties or description.
    void in_scope(const string_map & addrbook, bool is_direct, bool is_static);

};

#endif

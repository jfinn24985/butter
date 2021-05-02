#ifndef _UMLEXPANSIONNODE_H
#define _UMLEXPANSIONNODE_H


#include "bouml/UmlBaseExpansionNode.h"
#include <qbytearray.h>

class UmlExpansionNode : public UmlBaseExpansionNode {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlExpansionNode(void * id, const QByteArray & s) : UmlBaseExpansionNode(id, s) {
    }

};

#endif

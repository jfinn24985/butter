#ifndef _UMLJUNCTIONPSEUDOSTATE_H
#define _UMLJUNCTIONPSEUDOSTATE_H


#include "bouml/UmlBaseJunctionPseudoState.h"

class UmlJunctionPseudoState : public UmlBaseJunctionPseudoState {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlJunctionPseudoState(void * id) : UmlBaseJunctionPseudoState(id, "") {
    }

};

#endif

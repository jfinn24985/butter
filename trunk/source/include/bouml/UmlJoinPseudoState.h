#ifndef _UMLJOINPSEUDOSTATE_H
#define _UMLJOINPSEUDOSTATE_H


#include "bouml/UmlBaseJoinPseudoState.h"

class UmlJoinPseudoState : public UmlBaseJoinPseudoState {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlJoinPseudoState(void * id) : UmlBaseJoinPseudoState(id, "") {
    }

};

#endif

#ifndef _UMLSEQUENCEDIAGRAM_H
#define _UMLSEQUENCEDIAGRAM_H


#include "bouml/UmlBaseSequenceDiagram.h"
#include <qcstring.h>

/**
 *  This class manages 'sequence diagrams', notes that the class 'UmlDiagram'
 *  is a mother class of all the diagrams, allowing to generalize their
 *  management
 * 
 *  You can modify it as you want (except the constructor)
 */
class UmlSequenceDiagram : public UmlBaseSequenceDiagram {
  public:
    UmlSequenceDiagram(void * id, const QCString & n) : UmlBaseSequenceDiagram(id, n) {};

};

#endif

#ifndef _UMLACTIVITYDIAGRAM_H
#define _UMLACTIVITYDIAGRAM_H


#include "bouml/UmlBaseActivityDiagram.h"
#include "bouml/UmlActivityItem.h"
#include <qbytearray.h>

/**
 *  This class manages 'activity diagrams', notes that the class 'UmlDiagram'
 *  is a mother class of all the diagrams, allowing to generalize their
 *  management
 * 
 *  You can modify it as you want (except the constructor)
 */
class UmlActivityDiagram : public UmlBaseActivityDiagram, public UmlActivityItem {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlActivityDiagram(void * id, const QByteArray & s) : UmlBaseActivityDiagram(id, s) {
    }

};

#endif

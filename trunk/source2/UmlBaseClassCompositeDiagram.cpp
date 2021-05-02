
#include "UmlBaseClassCompositeDiagram.h"
#include "UmlClassCompositeDiagram.h"
#include "UmlClass.h"

//  returns a new class composite diagram named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlClassCompositeDiagram * UmlBaseClassCompositeDiagram::create(UmlClass * parent, const char * s)
{
    return (UmlClassCompositeDiagram *) parent->create_(aClassCompositeDiagram, s);
}

// returns the kind of the item
anItemKind UmlBaseClassCompositeDiagram::kind() {
    return aClassCompositeDiagram;
}


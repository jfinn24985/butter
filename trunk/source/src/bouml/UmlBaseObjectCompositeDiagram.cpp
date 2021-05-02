
#include "bouml/UmlBaseObjectCompositeDiagram.h"
#include "bouml/UmlObjectCompositeDiagram.h"
#include "bouml/UmlClassInstance.h"

/**
 *   returns a new object composite diagram named 's' created under 'parent'
 * 
 *  In case it cannot be created (the name is already used or
 *  invalid, 'parent' cannot contain it etc ...) return 0 in C++
 *  and produce a RuntimeException in Java
 */
UmlObjectCompositeDiagram * UmlBaseObjectCompositeDiagram::create(UmlClassInstance * parent, const char * s)
{
    return (UmlObjectCompositeDiagram *) parent->create_(aClassCompositeDiagram, s);
}

/**
 *  returns the kind of the item
 */
anItemKind UmlBaseObjectCompositeDiagram::kind() {
    return aClassCompositeDiagram;
}


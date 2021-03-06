#ifndef _UMLPARAMETER_H
#define _UMLPARAMETER_H


#include "bouml/aDirection.h"
#include "bouml/UmlTypeSpec.h"
#include <qcstring.h>

/**
 *   Represent an operation's parameter
 */
struct UmlParameter {
    aDirection dir;

    UmlTypeSpec type;

    QCString name;

    QCString default_value;

};

#endif

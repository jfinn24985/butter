#ifndef _UMLPACKAGE_H
#define _UMLPACKAGE_H


#include "UmlBasePackage.h"
#include <qbytearray.h>
#include <qstring.h>
#include <qvector.h>

class UmlItem;

// This class manages a 'package', notes that the project itself is a package
//
// You can modify it as you want (except the constructor)
class UmlPackage : public UmlBasePackage {
  public:
    UmlPackage(void * id, const QByteArray & n) : UmlBasePackage(id, n) {};

    //Find UmlArtifact objects of the given name at the current location. This
    //may return an empty list if none are found
    //
    //\pre not name.empty
    void find_uml_document(QString name, QVector< UmlItem* > & result);

};

#endif

#ifndef _UMLPACKAGE_H
#define _UMLPACKAGE_H


#include "bouml/UmlBasePackage.h"
#include <qstring.h>
#include <qdir.h>

#include <qcstring.h>

class CppSettings;

/**
 *  This class manages a 'package', notes that the project itself is a package
 * 
 *  You can modify it as you want (except the constructor)
 */
class UmlPackage : public UmlBasePackage {
  public:
    /**
     * I use POSIX terminology for paths.  In POSIX a "filename" is every
     * part of a path between separators as well as the part after the 
     * last separator.  In the context of a package, this will be the 
     * directory name of the src directory for the package.
     */
    QString filename() const
    {
      return QDir (const_cast< UmlPackage* > (this)->cppSrcDir ()).dirName ();
    }
    

    /**
     * Create the absolute path to the header directory for this package
     */
    QDir hdr_path() const;
    /**
     * Create the absolute path to the source directory for this package
     */
    QDir src_path() const;
    UmlPackage(void * id, const QCString & n) : UmlBasePackage(id, n) {};


  private:
    /**
     * Path to project root.
     */
    static QDir* base_;

    /**
     * Set the base pointer
     */
    void _set_base() const;

};

#endif


#include "bouml/UmlArtifact.h"

#include <qdir.h>
#include <qmessagebox.h>
#include <qtextstream.h>

#include "CppSettings.h"
#include "UmlPackage.h"
#include "UmlCom.h"
#include "UmlClass.h"

#ifdef ASDFG
static QCString root_dir()
{
  static QCString RootDir;

  if (RootDir.isEmpty()) {
    RootDir = CppSettings::rootDir();
    
    if (RootDir.isEmpty()) {
      QMessageBox::critical((QWidget *) 0, "Error", "Generation directory must be set");
      throw 0;
    }
  
    if (QDir::isRelativePath(RootDir)) {
      QFileInfo f(UmlPackage::getProject()->supportFile());
      QDir d(f.dirPath());
      
      RootDir = d.filePath(RootDir);
    }
  }
  
  return RootDir;
}
#endif


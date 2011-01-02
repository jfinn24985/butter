
#include "bouml/UmlPackage.h"
#include "bouml/CppSettings.h"

QDir UmlPackage::hdr_path() const 
{
if (! base_)
{
  _set_base ();
}
return QDir (base_->path().append(QDir::separator()).append(const_cast< UmlPackage & > (*this).cppHDir ()));

}

QDir UmlPackage::src_path() const 
{
if (! base_)
{
  _set_base ();
}
return QDir (base_->path().append(QDir::separator()).append(const_cast< UmlPackage & > (*this).cppSrcDir ()));

}

/**
 * Path to project root.
 */
QDir* UmlPackage::base_;

void UmlPackage::_set_base() const 
{
if (NULL == base_)
{
   base_ = new QDir (CppSettings::rootDir ());
   if (base_->isRelative ())
   {
     UmlItem *p_ = const_cast< UmlPackage* >(this);
     while (p_->parent () != NULL) { p_ = p_->parent (); }
     QFileInfo f_ (dynamic_cast< UmlPackage* >(p_)->supportFile());
     base_->setPath(f_.dir().filePath(base_->path ()));
   }
}

}


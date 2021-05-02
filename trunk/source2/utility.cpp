
#include "utility.h"


#include <config.h>
#include <iostream>

namespace butter {

std::ostream & operator<<(std::ostream & os, const QString &s)
{
  os << s.toStdString();
  return os;
}
//Separator to use regardless of OS

const char enumerate_path::default_separator_[2]= "/";

//separator used on other OS

const char enumerate_path::other_separator_[2]= "\\";


// Lifetime methods

void enumerate_path::swap(enumerate_path & other) {
  QString t(this->path());
  this->setPath(other.path());
  other.setPath(t);
}


// Access methods

//Test if the path starts with a drive letter as in "?:"
bool enumerate_path::is_dos_absolute() const {
  if(this->depth() > 0)
  {
    QString s{ this->leaf_at(0) };
    if(s.size() > 1 and s[1] == QChar(':'))
    {
      return true;
    }
  }
  return false;
}

//The number of steps in the path.
unsigned int enumerate_path::depth() const {
  const QString path_(path());
  return (unsigned int)(path_.count(default_separator_[0]) + ((path_.lastIndexOf(default_separator_[0]) + 1 == path_.size()) ? 0 : 1));
}

//Here subpath means that a_rhs is a subdirectory or file in this
//directory.
//
//NOTE: a is_subpath implies that this == create_common(a_rhs).

bool enumerate_path::has_subpath(const enumerate_path & a_rhs) const {
// simple test that a subdirs path string must have longer pathname
const QString lhs_ = absolutePath();
const QString rhs_ = a_rhs.absolutePath ();
bool Result(lhs_.length () < rhs_.length ());
// Test if rhs_ begins with lhs_
if( Result )
{
  Result = rhs_.startsWith (lhs_);
}
return Result;

}

//Compare two paths. Returns canonicalPath = a_rhs.canonicalPath 
//(ignoring trailing separator)
bool enumerate_path::equality(const enumerate_path & a_rhs) const {
const QString lhs_ = absPath ();
const QString rhs_ = a_rhs.absPath ();
// Do simple comparison first
if (lhs_ == rhs_)
{
  return true;
}
const unsigned int lhs_sz_ = lhs_.length ();
const unsigned int rhs_sz_ = rhs_.length ();
switch (lhs_sz_ - rhs_sz_)
{
// check if lhs_ ends in redundant separator
case 1:
{
  if (default_separator_[0] == lhs_.at (lhs_sz_ - 1))
  {
    return lhs_.startsWith (rhs_);
  }
  break;
}
// check if rhs_ ends in redundant separator
case -1:
{
  if (default_separator_[0] == rhs_.at (rhs_sz_ - 1))
  {
    return rhs_.startsWith (lhs_);
  }
  break;
}
default:
  break;
}
return false;

}

//Get part of path at position a_count
QString enumerate_path::leaf_at(unsigned int a_count) const {
int s_ = 0, e_ = 0; // start, end counter
QString path_(path());
while (true)
{
  e_ = path_.find(default_separator_[0], s_);
  if (e_ < 0) // End of path string, check at last item
  {
    if (a_count != 0)
    {
      throw std::runtime_error ("Attempt to go beyond end of path.");
    }
    e_ = path_.length();
  }
  if (a_count == 0)
  {
    return path_.mid(s_, e_ - s_);
  }
  s_ = e_ + 1;
  --a_count;
}
}

//Generate a path that has the greatest shared path between this
//and a_other.
//
//Will raise and exception when paths are on different drives.
//
//**NOTE: This class is not intended as a general solution.
//In the butter program having two paths that have no common root
//is an exception as we can not create a build system without
//the common root.  This is why this method throws an exception**

QString enumerate_path::create_common(const enumerate_path & a_other) const {
if(this->is_dos_absolute() and a_other.is_dos_absolute() and this->leaf_at(0) != a_other.leaf_at(0))
{
  throw std::domain_error( ("Path \""+path()+"\" and \""+a_other.path()+"\" have no common parent.").toStdString() );
}
const enumerate_path lhs_{ this->is_dos_absolute() ? this->path() : (this->isRelative() ? this->absolutePath() : this->path()) };
const enumerate_path rhs_{ a_other.is_dos_absolute() ? a_other.path() : (a_other.isRelative() ? a_other.absolutePath() : a_other.path()) };
if(lhs_.is_dos_absolute() and rhs_.is_dos_absolute() and lhs_.leaf_at(0) != rhs_.leaf_at(0))
{
  return QString();
}
const unsigned int depth_{ std::min(lhs_.depth(), rhs_.depth()) };
enumerate_path result_{ lhs_.is_dos_absolute() ? lhs_.leaf_at(0) : default_separator_[0] + lhs_.leaf_at(0) };
for(unsigned int i_ = 1; i_ != depth_; ++i_)
{
  if( lhs_.leaf_at(i_) != rhs_.leaf_at(i_) )
  {
    // common upto leaf (i_ - 1)
    return result_.path();
  }
  result_ = result_ / lhs_.leaf_at(i_);
}
return result_.path();
}

//Build a relative path from here to a_target.
//
//canonical (path_ + relative (a_target)) <==> a_target
QString enumerate_path::create_relative(const enumerate_path & a_target) const {
const enumerate_path lhs_{ this->is_dos_absolute() ? this->path() : (this->isRelative() ? this->absolutePath() : this->path()) };
const enumerate_path rhs_{ a_target.is_dos_absolute() ? a_target.path() : (a_target.isRelative() ? a_target.absolutePath() : a_target.path()) };
if(not (lhs_.is_dos_absolute() or rhs_.is_dos_absolute()))
{
  return normalise(lhs_.relativeFilePath(rhs_.path()));
}
else
{
#ifndef NO_ARRAY_INITIALISER
  const char sep_[4] = { '.', '.', default_separator_[0], '\0' };
#else
  char sep_[4];
  sep_[0] = '.';
  sep_[1] = '.';
  sep_[2] = default_separator_[0];
  sep_[3] = '\0';
#endif
  enumerate_path result_;
  const enumerate_path common_{ lhs_.create_common(rhs_) };
  unsigned int start{ "." == common_.path() ? 0 : common_.depth() };
  for(unsigned int i = 0; i != (lhs_.depth() - start); ++i)
  {
    if(0 == i)
    {
      result_.setPath(sep_);
    }
    else
    {
      result_ = result_ / sep_;
    }
  }
  if(0 == result_.depth())
  {
    return rhs_.path();
  }
  for(unsigned int i = start; i != rhs_.depth(); ++i)
  {
    result_ = result_ / rhs_.leaf_at(i);
  }
  return result_.path();
}
}


// Modification methods

//Convert string to form with all separators as "/" (function part)
QString enumerate_path::normalise(QString a_path)
{
return QDir::cleanPath(a_path.replace(QRegExp("\\\\"), default_separator_));
}

//Create a copy of the path string that has all the separators replaced 
//by some arbitrary string "a_trans".
QString enumerate_path::path_convert(QString a_trans) const {
return path ().replace (QRegExp (default_separator_), a_trans);

}


// Modify filesystem methods

//Create this and all parent directories for the current path.
bool enumerate_path::mkpath() const {
// std::stack < QString > stack_; // Stores needed path
// QString path_ (normalise (path ()));  // Current path as string
// QDir cursor_;  // Current path
// log::com.trace (log::debug, ("<p>Creating path [" + path_ + "]</p>").utf8());
// int left_ (path_.findRev (default_separator_));
// for ( ; (!cursor_.exists (path_, true)) & (!path_.isEmpty ()) & (left_ > 0); )
// {
//   stack_.push (path_.mid (left_ + 1));
//   path_.truncate (left_);
//   left_ = path_.findRev (default_separator_);
// }
// cursor_.cd (path_, true);
// if (DEBUG)
// {
//   log::com.trace (log::debug, ("<p>Found path [" + path_ + "] exists </p>").utf8());
// }
// for ( ; !stack_.empty ()
//          && cursor_.mkdir (stack_.top (), false)
//          && cursor_.cd (stack_.top (), false)
//       ; stack_.pop ()) {} // Empty loop
// return stack_.empty ();
// 
return false;
}


} // namespace butter

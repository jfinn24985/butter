/**
 * Utility classes log and pathcmp
 */
#include "butter/utility.h"
#include "bouml/UmlCom.h"

#include <stdexcept>
#include <stack>
#include <qregexp.h>
#include "butter/config.h"
namespace butter {

QString pathcmp::create_common(const pathcmp & a_other) const 
{
// Bouml preserved body begin 0002D6A9
const QString lhs_ = absPath ();
const QString rhs_ = a_other.absPath ();
const int sz1_ = lhs_.length ();
const int sz2_ = rhs_.length ();
const int sz_ = (sz1_ < sz2_) ? sz1_ : sz2_;
int last_sep_ = 0;
for (int c_ = 0; c_ < sz_; ++c_)
{
  if (default_separator_[0] == lhs_[c_])
  {
    last_sep_ = c_;
  }
  if (lhs_[c_] != rhs_[c_])
  {
    // End of common
    return (last_sep_ > 0)
    	? lhs_.left (last_sep_)
    	: QString ();
  }
}
// Matched all of smaller path.
return lhs_.left (sz_);
// Bouml preserved body end 0002D6A9

}

QString pathcmp::create_relative(const pathcmp & a_target) const 
{
// Bouml preserved body begin 0002D7A9
QString Result;
#ifndef NO_ARRAY_INITIALISER
const char sep_[4] = { '.', '.', default_separator_[0], '\0' };
#else
char sep_[4];
sep_[0] = '.';
sep_[1] = '.';
sep_[2] = default_separator_[0];
sep_[3] = '\0';
#endif
const QString path1_ = absPath ();
const QString path2_ = a_target.absPath ();
const int sz1_ (path1_.length ());
const int sz2_ (path2_.length ());
const int sz_ = (sz1_ < sz2_) ? sz1_ : sz2_;
// Determine the parent dirs in common.
int last_sep_ = 0;
for (int i_ = 0; true; ++i_)
{
  if (default_separator_[0] == path1_[i_])
  {
    last_sep_ = i_;
  }
  if (i_ >= sz_)
  { // End of shorter path
    last_sep_ = i_;
    break;
  }
  if (path1_[i_] != path2_[i_])
  { // End of common path
    break; 
  }
}
#ifdef _WS_WIN_
// Throw exception when the two directories are not on the same partition
if (0 == last_sep_)
{
  if (default_separator_[0] != path1_[last_sep_])
  {
    throw std::runtime_error (("<p><b>Configuration error</b> Path " 
                       + path1_ + " and " + path2_ + " are not on the same partition.").utf8().data ());
  }
}
#endif
// Append "up dirs" for each separator between last_sep_ and end.
for (int ii_ = path1_.right (sz1_ - last_sep_).contains (default_separator_[0]); ii_ > 0; --ii_)
{
  Result.append (&sep_[0]);
}
if (sz2_ > last_sep_)
{
  // Add down elements from common to a_target
  Result.append (path2_.right (sz2_ - last_sep_ - 1));
}
else
{
  // No down elements, remove trailing separator
  Result.truncate (Result.length () - 1);
}
return Result;
// Bouml preserved body end 0002D7A9

}

bool pathcmp::equality(const pathcmp& a_rhs) const 
{
// Bouml preserved body begin 0002D5A9
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
// Bouml preserved body end 0002D5A9

}

bool pathcmp::has_subpath(const pathcmp & a_rhs) const 
{
// Bouml preserved body begin 0002D629
// simple test that a subdirs path string must have longer pathname
const QString lhs_ = absPath ();
const QString rhs_ = a_rhs.absPath ();
bool Result (lhs_.length () < rhs_.length ());
// Test if rhs_ begins with lhs_
if (Result)
{
  Result = rhs_.startsWith (lhs_);
}
return Result;
// Bouml preserved body end 0002D629

}

QString pathcmp::leaf_at(unsigned int a_count) const 
{
// Bouml preserved body begin 0002D3A9
int s_ = 0, e_ = 0; // start, end counter
QString path_ (path ());
while (true)
{
  e_ = path_.find (default_separator_[0], s_);
  if (e_ < 0) // End of path string, check at last item
  {
    if (a_count != 0)
    {
      throw std::runtime_error ("Attempt to go beyond end of path.");
    }
    e_ = path_.length ();
  }
  if (a_count == 0)
  {
    return path_.mid (s_, e_ - s_);
  }
  s_ = e_ + 1;
  --a_count;
}
// Bouml preserved body end 0002D3A9

}

bool pathcmp::mkpath() const 
{
// Bouml preserved body begin 0003A9A9
std::stack < QString > stack_; // Stores needed path
QString path_ (normalise (path ()));  // Current path as string
QDir cursor_;  // Current path
log::com.trace (log::debug, ("<p>Creating path [" + path_ + "]</p>").utf8());
int left_ (path_.findRev (default_separator_));
for ( ; (!cursor_.exists (path_, true)) & (!path_.isEmpty ()) & (left_ > 0); )
{
  stack_.push (path_.mid (left_ + 1));
  path_.truncate (left_);
  left_ = path_.findRev (default_separator_);
}
cursor_.cd (path_, true);
if (DEBUG)
{
  log::com.trace (log::debug, ("<p>Found path [" + path_ + "] exists </p>").utf8());
}
for ( ; !stack_.empty ()
         && cursor_.mkdir (stack_.top (), false)
         && cursor_.cd (stack_.top (), false)
      ; stack_.pop ()) {} // Empty loop
return stack_.empty ();
// Bouml preserved body end 0003A9A9

}

QString pathcmp::normalise_(QString a_path)
{
  return a_path.replace (QRegExp("\\\\"), default_separator_);
}
QString pathcmp::path_convert(QString a_trans) const 
{
// Bouml preserved body begin 00034D29
return path ().replace (QRegExp (default_separator_), a_trans);
// Bouml preserved body end 00034D29

}

/**
 * Separator to use regardless of OS
 */
const char pathcmp::default_separator_[2] = "/";

/**
 * separator used on other OS
 */
const char pathcmp::other_separator_[2] = "\\";

/**
 * Exemplar
 */
log log::com;
void log::debug_log(const char * a_fname) 
{
BUTTER_ALWAYS (DEBUG, "Programming error: attempt to set debug log in release build");
// Bouml preserved body begin 00032A29
debug_file_ = std::auto_ptr< QFile >(new QFile (a_fname));
if (! debug_file_->open (IO_WriteOnly))
{
  QString msg_ ("<p>Unable to open log file for read/writing: ");
  msg_.append (debug_file_->name ());
  msg_.append ("</p>");
  debug_os_ = std::auto_ptr< QTextOStream >(); // Reset stream
  throw std::runtime_error (msg_.utf8 ().data ());
}
else
{
  debug_os_ = std::auto_ptr< QTextOStream >(new QTextOStream (debug_file_->handle ()));
}
// Bouml preserved body end 00032A29

}
log::~log() 
{
// Bouml preserved body begin 00036729
// NOTE: This will only be called during program exit.

// For some reason under WIN32 we get a violation error, when we
// reset the pointer, so here we call the dtor and then release
// the pointers.
if (DEBUG)
{
  if (NULL != debug_os_.get ())
  {
    debug_os_->~QTextOStream ();
    debug_os_.release ();
  }
  if (NULL != debug_file_.get ())
  {
    debug_file_->flush ();
    debug_file_->~QFile ();
    debug_file_.release ();
  }
}
// Bouml preserved body end 00036729

}

::QTextOStream & log::stream() const 
{
  BUTTER_ALWAYS (DEBUG, "Programming error: attempt to get debug log in release build");
  return *debug_os_;
}
void log::trace(log::log_levels a_lvl, const char * a_msg) 
{
  if (level_ >= a_lvl) UmlCom::trace (a_msg);
}

} // namespace butter

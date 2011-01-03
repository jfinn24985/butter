
#include "butter/compound_artifact.h"
#include "butter/butter_constants.h"
#include "butter/utility.h"
#include "butter/style.h"
#include "bouml/UmlArtifact.h"
#include "butter/basic_style.h"

// Manual includes
#include <stdexcept>
#include "butter/config.h"
// --
namespace butter {

compound_artifact::compound_artifact(::UmlArtifact & a_art) 
// Bouml preserved body begin 00039B29
: artifact (a_art)
, close ()
, date ()
, end ()
, preamble ()
, style_(butter::style::get_style())
, targets_ ()
, target_order_ ()
, version ()
{
#if QT_VERSION < 300L
  QString tmp_ (a_art.description ());
#else
  const QString tmp_ (a_art.description ());
#endif
  QTextIStream is_ (&tmp_);
  deserialise (is_);
}
// Bouml preserved body end 00039B29


compound_artifact::compound_artifact(::UmlArtifact & a_art, const basic_style & a_style) 
// Bouml preserved body begin 0002FCA9
: artifact (a_art)
, close ()
, date ()
, end ()
, preamble ()
, style_(a_style)
, targets_ ()
, target_order_ ()
, version ()
{
#if QT_VERSION < 300L
  QString tmp_ (a_art.description ());
#else
  const QString tmp_ (a_art.description ());
#endif
  QTextIStream is_ (&tmp_);
  deserialise (is_);
}
// Bouml preserved body end 0002FCA9


compound_artifact::~compound_artifact() 
{
// Bouml preserved body begin 0002FD29
// Compare to existing document
QString desc_;
const QString orig_desc_ (artifact.description ());
QTextOStream os_(&desc_);
serialise (os_);
if (0 != orig_desc_.compare (desc_))
{
  // Document changed, update version and date
  version.second = style_.comment_text (butter_constants::BUTTER_VERSION);
  date.second = style_.comment_text (QString (QDateTime::currentDateTime ().toString ()));
  QString desc2_;
  QTextOStream os2_(&desc2_);
  serialise (os2_);
  artifact.set_Description (desc2_.utf8 ());
}
// Bouml preserved body end 0002FD29

}

void compound_artifact::deserialise(::QTextIStream & a_is) 
{
// Bouml preserved body begin 0002F229
QString capture_;
int state_ = 0; // 0 = unlabelled, 1 = labelled
string_pair_t * cache_ = NULL;
for (QString line_ = a_is.readLine ();
    ! line_.isNull (); // Check for EOF
    line_ = a_is.readLine ())
{
  switch (state_)
  {
  case 0: // Unlabelled
    {
      if (line_.startsWith (style_.start_phrase))
      {
        if (line_.contains (QString (butter::butter_constants::version_label)))
        {
          cache_ = &version;
        }
        else if (line_.contains (QString (butter::butter_constants::date_label)))
        {
          cache_ = &date;
        }
        else if (line_.contains (QString (butter::butter_constants::preamble_label)))
        {
          cache_ = &preamble;
        }
        else if (line_.contains (QString (butter::butter_constants::close_label)))
        {
          cache_ = &close;
        }
        else if (line_.contains (QString (butter::butter_constants::target_label)))
        {
          QString label_ = line_.mid (line_.find (QString (butter::butter_constants::target_label)) + butter::butter_constants::target_label.length ());
          cache_ = &target (label_.simplifyWhiteSpace ());
        }
        else
        {
          QString name_ ("<p><b>Fatal Error:</b> Error in artifact <b>");
          name_.append ((pathcmp(artifact.parent ()->parent ()->name ()) / artifact.name ()).path ());
	  name_.append ("</b>, appears to have an unknown section type.</p>");
          throw std::runtime_error (name_.utf8 ().data ());
        }
        if (! capture_.isEmpty ())
        {
          cache_->first = capture_;
          capture_.setUnicode (0, 0);
        }
        state_ = 1;
      }
      else if (line_.startsWith (style_.end_phrase))
      {
        QString name_ ("<p><b>Fatal Error:</b> Error in artifact <b>");
        name_.append ((pathcmp(artifact.parent ()->parent ()->name ()) / artifact.name ()).path ());
	name_.append ("</b>, section end outside a section</p>");
        throw std::runtime_error (name_.utf8 ().data ());
      }
      else
      {
        capture_ += line_;
        capture_ += '\n';
      }
    }
    break;
  case 1: // In a section
    {
      if (line_.startsWith (style_.end_phrase))
      {
        if (! capture_.isEmpty ())
        {
          cache_->second = capture_;
          capture_.setUnicode (0, 0);
        }
        state_ = 0;
      }
      else if (line_.startsWith (style_.start_phrase))
      {
        throw std::runtime_error ("<p>Badly formed artifact with section start inside a section</p>");
      }
      else
      {
        capture_ += line_;
        capture_ += '\n';
      }
    }
    break;
  default: // Error!
    throw std::runtime_error ("<p>Programming error: at a default switch case.</p>");
    break;
  }
}
if (! capture_.isEmpty ())
{
  if (NULL != cache_) // Have there been sections?
  {
    end = capture_;
  }
  else
  {
    version.first = capture_;
  }
}
// Bouml preserved body end 0002F229

}

bool compound_artifact::has_target(QString a_label) const 
{
// Bouml preserved body begin 00030829
return NULL != targets_.find (a_label);
// Bouml preserved body end 00030829

}

void compound_artifact::merge(::UmlArtifact & a_art) 
{
// Bouml preserved body begin 0002FAA9
class helper_
{
public:
  static void merge_ (string_pair_t &targ_, const string_pair_t &src_)
  {
    if (! src_.first.isEmpty ())
    {
      targ_.first.append (src_.first);
    }
    if (! src_.second.isEmpty ())
    {
      targ_.second.append (src_.second);
    }
  }
};
const butter::compound_artifact tmp_ (a_art, style_);
helper_::merge_ (version, tmp_.version);
helper_::merge_ (date, tmp_.date);
helper_::merge_ (preamble, tmp_.preamble);
helper_::merge_ (close, tmp_.close);
for (QDictIterator< string_pair_t > i_ (tmp_.targets_);
     NULL != i_.current ();
     ++i_)
{
  helper_::merge_ (target (i_.currentKey ()), *i_.current ());
}
end.append (tmp_.end);
// Bouml preserved body end 0002FAA9

}

void compound_artifact::reset() 
{
// Bouml preserved body begin 0002FEA9
if (! targets_.isEmpty ())
{
  for (QDictIterator< string_pair_t > i_ (targets_);
      NULL != i_.current ();
      ++i_)
  {
    i_.current ()->second.truncate (0);
  }
}
if (! preamble.second.isEmpty ())
{
  preamble.second.truncate (0);
}
if (! close.second.isEmpty ())
{
  close.second.truncate (0);
}
// Bouml preserved body end 0002FEA9

}

void compound_artifact::serialise(::QTextOStream & a_os) const 
{
// Bouml preserved body begin 0002F1A9
class helper_
{
public:
  static void write (QTextStream &a_os_, const string_pair_t & a_cache_, QString a_label_, const butter::basic_style & s_)
  {
    if (! a_cache_.first.isEmpty ())
    {
      a_os_ << a_cache_.first;
    }
    if (! a_cache_.second.isEmpty ())
    {
      a_os_ << s_.start_phrase << a_label_ << s_.end_start_phrase << "\n";
      a_os_ << a_cache_.second;
      a_os_ << s_.end_phrase << a_label_ << s_.end_end_phrase << "\n";
    }
  }
};
helper_::write (a_os, version, butter::butter_constants::version_label, style_);
helper_::write (a_os, date, butter::butter_constants::date_label, style_);
helper_::write (a_os, preamble, butter::butter_constants::preamble_label, style_);
for (QValueListConstIterator< QString > i_ (target_order_.begin ());
      target_order_.end () != i_;
      ++i_)
{
  string_pair_t * tmp_ = targets_.find (*i_);
  BUTTER_CHECK (NULL != tmp_, "Error, key in target_order_ not found in targets.");
  helper_::write (a_os, *tmp_, butter::butter_constants::target_label + " " + *i_, style_);
}
helper_::write (a_os, close, butter::butter_constants::close_label, style_);
if (! end.isEmpty ())
{
  a_os << end;
}
// Bouml preserved body end 0002F1A9

}

compound_artifact::string_pair_t & compound_artifact::target(QString a_label) 
{
// Bouml preserved body begin 000305A9
string_pair_t * existing_ = targets_.find (a_label);
if (NULL != existing_)
{
  return *existing_;
}
std::auto_ptr< string_pair_t > newitem_ (new string_pair_t);
target_order_.append (a_label);
targets_.insert (a_label, newitem_.get ());
return *newitem_.release ();
// Bouml preserved body end 000305A9

}


} // namespace butter

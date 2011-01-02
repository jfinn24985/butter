
#include "butter/style.h"
#include "butter/bjam_generator.h"
#include "butter/cmake_generator.h"
#include "butter/gmake_generator.h"
#include "butter/utility.h"
#include "butter/jam_generator.h"
#include "butter/base_generator.h"

// Manual include
#include <qtextstream.h>
//--
namespace butter {

QString basic_style::comment_text(QString a_text) const 
{
QString Result;
QTextOStream os_ (&Result);
int s_ = 0; // start position
for (int e_ = a_text.find ('\n', s_); e_ > 0; e_ = a_text.find ('\n', s_))
{
  os_ << comment_string;
  os_ << a_text.mid (s_, e_ - s_ - 1);
  os_ << end_comment_string << "\n";
  s_ = e_ + 1;
}
if (a_text.at (a_text.length () - 1) != '\n')
{
  os_ << comment_string;
  os_ << a_text.mid (s_);
  os_ << end_comment_string << "\n";
}
return Result;

}

/**
 * Singleton object.
 */
std::auto_ptr< style > style::style_;

std::auto_ptr< base_generator > style::create_writer() const 
{

  return (*factory_)();
}

const style& style::get_style()

{
BUTTER_REQUIRE (NULL != style_.get (), "Programming error, attempt to get style before it was set.");
return *style_.get ();

}

void style::set_style(QString a_style)
{
if (a_style == butter::bjam_generator::section_name)
{
  MSC_NO_RESET(style_, std::auto_ptr< style >)(new style (butter::bjam_generator::comment_string
                     , butter::bjam_generator::end_phrase
                     , butter::bjam_generator::start_phrase
                     , butter::bjam_generator::section_name
                     , &butter::bjam_generator::create));
  log::com.trace (log::info, "<p>Using style <b>Boost Jam</b></p>");
}
else if (a_style == butter::cmake_generator::section_name)
{
  MSC_NO_RESET(style_, std::auto_ptr< style >)(new style (butter::cmake_generator::comment_string
                     , butter::cmake_generator::end_phrase
                     , butter::cmake_generator::start_phrase
                     , butter::cmake_generator::section_name
                     , &butter::cmake_generator::create));
  log::com.trace (log::info, "<p>Using style <b>CMake</b></p>");
}
else if (a_style == butter::gmake_generator::section_name)
{
  MSC_NO_RESET(style_, std::auto_ptr< style >)(new style (butter::gmake_generator::comment_string
                     , butter::gmake_generator::end_phrase
                     , butter::gmake_generator::start_phrase
                     , butter::gmake_generator::section_name
                     , &butter::gmake_generator::create));
  log::com.trace (log::info, "<p>Using style <b>Make</b></p>");
}
else
{
  if (a_style != butter::jam_generator::section_name)
  {
    QString msg_;
    {
      QTextOStream msg_os_ (&msg_);
      if (a_style.isEmpty ())
      {
        msg_os_ << "<p>No style set out of (";
      }
      else
      {
        msg_os_ << "<p><b>Warning</b> Style \"<b>" << a_style << "</b>\" does not match unrecognised (";
      }
      msg_os_ << butter::bjam_generator::section_name;
      msg_os_ << "|" << butter::jam_generator::section_name;
      msg_os_ << "|" << butter::cmake_generator::section_name;
      msg_os_ << "|" << butter::gmake_generator::section_name;
      msg_os_ << "), using <i>standard jam</i> as default</p>";
    }
    log::com.trace (log::warn, msg_);
  }
  else
  {
    log::com.trace (log::info, "<p>Using style <b>Standard Jam</b></p>");
  }
  MSC_NO_RESET(style_, std::auto_ptr< style >)(new style (butter::jam_generator::comment_string
                     , butter::jam_generator::end_phrase
                     , butter::jam_generator::start_phrase
                     , butter::jam_generator::section_name
                     , &butter::jam_generator::create));
}

}

style::style(QString a_comment, QString a_end, QString a_start, QString a_name, style::write_factory_fn_t a_factory) 
: basic_style (a_comment, "", a_end, "", a_start, "", a_name)
, factory_ (a_factory)
{}


style::~style() 
{

}


} // namespace butter

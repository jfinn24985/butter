
#include "butter/style.h"
#include "butter/utility.h"

// Manual include
#include <qtextstream.h>
#include "butter/config.h"
//--
namespace butter {

/**
 * Singleton object.
 */
std::auto_ptr< style > style::style_;

const style& style::get_style()

{
// Bouml preserved body begin 00030029
BUTTER_REQUIRE (NULL != style_.get (), "Programming error, attempt to get style before it was set.");
return *style_.get ();
// Bouml preserved body end 00030029

}

void style::set_style(QString a_style)
{
// Bouml preserved body begin 000300A9
extern const basic_style bjam_style;
extern const basic_style cmake_style;
extern const basic_style gmake_style;
extern const basic_style jam_style;
if (a_style == bjam_style.name)
{
  style_ = std::auto_ptr< style >(new style (bjam_style));
  log::com.trace (log::info, "<p>Using style <b>Boost Jam</b></p>");
}
else if (a_style == cmake_style.name)
{
  style_ = std::auto_ptr< style >(new style (cmake_style));
  log::com.trace (log::info, "<p>Using style <b>CMake</b></p>");
}
else if (a_style == gmake_style.name)
{
  style_ = std::auto_ptr< style >(new style (gmake_style));
  log::com.trace (log::info, "<p>Using style <b>Make</b></p>");
}
else
{
  if (a_style != jam_style.name)
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
        msg_os_ << "<p><b>Warning</b> Style \"<b>" << a_style << "</b>\" does not match recognised styles (";
      }
      msg_os_ << bjam_style.name;
      msg_os_ << "|" << cmake_style.name;
      msg_os_ << "|" << jam_style.name;
      msg_os_ << "|" << gmake_style.name;
      msg_os_ << "), using <i>standard jam</i> as default</p>";
    }
    log::com.trace (log::warn, msg_);
  }
  else
  {
    log::com.trace (log::info, "<p>Using style <b>Standard Jam</b></p>");
  }
  style_ = std::auto_ptr< style >(new style (jam_style));
}
// Bouml preserved body end 000300A9

}

style::style(QString a_comment, QString a_end, QString a_start, QString a_name, basic_style::write_factory_fn_t a_factory) 
// Bouml preserved body begin 00030629
: basic_style (a_comment, "", a_end, "", a_start, "", a_name, a_factory)
{}
// Bouml preserved body end 00030629


style::~style() 
{
// Bouml preserved body begin 000306A9
// Bouml preserved body end 000306A9

}


} // namespace butter


#include "style.h"
#include "basic_style.h"

namespace butter {

//Singleton object.

const basic_style * style::style_;

//Get the Singleton Style object. set_style must have been called
//prior to calling this method.
basic_style style::get_style()
{
BUTTER_REQUIRE( nullptr != style_, "Programming error, attempt to get style before it was set." );
return *style_;

}

//Set the style parameters.
void style::set_style()
{
if (a_style == butter::bjam_generator::style.name())
{
  style_ = &butter::bjam_generator::style;
  log::com.trace (log::info, "<p>Using style <b>Boost Jam</b></p>");
}
else if (a_style == butter::cmake_generator::style.name())
{
  style_ = &butter::cmake_generator::style;
  log::com.trace (log::info, "<p>Using style <b>CMake</b></p>");
}
else if (a_style == butter::gmake_generator::style.name())
{
  style_ = &butter::gmake_generator::style;
  log::com.trace (log::info, "<p>Using style <b>Make</b></p>");
}
else
{
  if (a_style != butter::jam_generator::style.name())
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
      msg_os_ << butter::bjam_generator::style.name();
      msg_os_ << "|" << butter::cmake_generator::style.name();
      msg_os_ << "|" << butter::jam_generator::style.name();
      msg_os_ << "|" << butter::gmake_generator::style.name();
      msg_os_ << "), using <i>standard jam</i> as default</p>";
    }
    log::com.trace (log::warn, msg_);
  }
  else
  {
    log::com.trace (log::info, "<p>Using style <b>Standard Jam</b></p>");
  }
  style_ = &butter::jam_generator::style;
}

}


} // namespace butter

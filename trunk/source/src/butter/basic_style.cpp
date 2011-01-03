
#include "butter/basic_style.h"
#include "butter/base_generator.h"

#include <qtextstream.h>
namespace butter {

QString basic_style::comment_text(QString a_text) const 
{
// Bouml preserved body begin 0002FC29
QString Result;
{
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
}
return Result;
// Bouml preserved body end 0002FC29

}

std::auto_ptr< base_generator > basic_style::create_writer() const 
{
  return (*factory)();
}


} // namespace butter

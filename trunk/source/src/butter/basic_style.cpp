
#include "butter/basic_style.h"
#include "butter/base_generator.h"

#include <qtextstream.h>
namespace butter {

QString basic_style::comment_text(QString a_text) const 
{
QString Result;
{
  QTextOStream os_ (&Result);
  int s_ = 0; // start position
  for (int e_ = a_text.find ('\n', s_); e_ > 0; e_ = a_text.find ('\n', s_))
  {
    os_ << comment_string_;
    os_ << a_text.mid (s_, e_ - s_);
    os_ << end_comment_string_ << "\n";
    s_ = e_ + 1;
  }
  if (a_text.at (a_text.length () - 1) != '\n')
  {
    os_ << comment_string_;
    os_ << a_text.mid (s_);
    os_ << end_comment_string_ << "\n";
  }
}
return Result;

}

std::unique_ptr< base_generator > basic_style::create_writer() const 
{
  return (*factory_)();
}


} // namespace butter


#include "UmlItem.h"

 UmlItem::~UmlItem() {
}

//Attempt to get butter relevant property values from the object properties or description.
void UmlItem::in_scope(const string_map & addrbook, bool is_direct, bool is_static) {
  // Process any data from properties.
  QString value_;
  for(auto label_ : { constants::butter_flags_name, 
      constants::butter_ldflags_name, 
      constants::butter_include_name } )
  {
    if (a_item.property_value(label_, value_))
    {
      addrbook.insert(label_, value_);
    }
  }
  // For non-documents also check for any data in the description.
  if( ! this->stereotype().contains(constants::document_stereotype))
  {
    QByteArray desc_{ this->description() };
    if( ! desc_.isEmpty() )
    {
      QString section_;
      const QByteArray marker_start_{ "${" + constants::section_prefix };
      const QByteArray marker_phrase_{ marker_start_ + a_section + "}" };
      const int start_{ desc_.find (marker_phrase_) };
      if (-1 != start_)
      {
        // Found a_section start, search from here to end for next section
        int end_{ desc_.find(marker_start_, start_ + marker_phrase_.length()) };
        if (-1 != end_)
        {
          section_ = desc_.mid(start_ + marker_phrase_.length(), end_ - (start_ + marker_phrase_.length()));
        }
        else
        {
          section_ = desc_.mid(start_ + marker_phrase_.length());
        }
        if (! section_.isEmpty ())
        {
          QRegExp hdr_re_("^ *" + constants::header_label + " *= *");
          QRegExp link_re_("^ *" + constants::link_label + " *= *");
          QRegExp flag_re_("^ *" + constants::flag_label + " *= *");
          if (! section_.isEmpty())
          {
            QTextIStream section_is_(&section_);
            while (! section_is_.atEnd())
            {
              const QString line_(section_is_.readLine());
              int length_;
              if (-1 != hdr_re_.match(line_, 0, &length_))
              {
                addrbook.insert(constants::butter_include_name, line_.mid(length_));
              }
              else if (-1 != link_re_.match(line_, 0, &length_))
              {
                addrbook.insert(constants::butter_ldflags_name, line_.mid(length_));
              }
              else if (-1 != flag_re_.match(line_, 0, &length_))
              {
                addrbook.insert(constants::butter_flags_name, line_.mid(length_));
              }
            }
          }
        }
      }
    }
  }
}



#include "build_style.h"
#include "text_template.h"

#include "config.h"
#include "constants.h"
#include <qdatetime.h>
#include <qtextstream.h>
#include <qvector.h>
#include <QXmlStreamReader>
namespace butter {

build_style::build_style()
: templates_()
, properties_()
, special_files_() {}

build_style::~build_style() {}

void build_style::read_style(QString style, QXmlStreamReader & xml) {
  BUTTER_REQUIRE(xml.isStartElement() && (xml.name() == constants::style_xmlname()),
                 ("XML reader not at a "+constants::style_xmlname()+" element in call to text_template::read.").toStdString());
  if(! this->properties_.empty())
  {
    this->properties_.clear();
  }
  if(! this->special_files_.empty())
  {
    this->special_files_.clear();
  }
  if(! this->templates_.empty())
  {
    this->templates_.clear();
  }
  {
    QVector< QString > phrases;
    bool found_style = false;
    while (!xml.atEnd())
    {
      switch(xml.readNext())
      {
      case QXmlStreamReader::StartElement:
      {
        // name = style | file | property | template
        if(xml.name() == constants::style_xmlname())
        {
          BUTTER_REQUIRE(xml.attributes().hasAttribute(constants::label_xmlname()),
                         QString("Style XML element does not have required attribute at line %1").arg(xml.lineNumber()).toStdString() );
          if(style != xml.attributes().value(constants::label_xmlname()) )
          {
            // skip to next style
            bool in_style = true;
            while (in_style && !xml.atEnd())
            {
              switch(xml.readNext())
              {
              case QXmlStreamReader::EndElement:
              {
                if(xml.name() == constants::style_xmlname())
                {
                  in_style = false;
                }
              }
              break;
              default:
                break;
              }
            }
          }
          else
          {
            found_style = true;
          }
        }
        else if(xml.name() == constants::text_template_xmlname())
        {
          BUTTER_REQUIRE(xml.attributes().hasAttribute(constants::label_xmlname()),
                         QString("Text template XML element does not have required attribute at line %1").arg(xml.lineNumber()).toStdString() );
          this->templates_[ xml.attributes().value(constants::label_xmlname()).toString() ].read( xml );
        }
        else if(xml.name() == constants::file_xmlname())
        {
          BUTTER_REQUIRE(xml.attributes().hasAttribute(constants::label_xmlname()),
                         QString("File XML element does not have required attribute at line %1").arg(xml.lineNumber()).toStdString() );
          QString & content = this->special_files_[ xml.attributes().value(constants::label_xmlname()).toString() ];
          bool in_file = true;
          while(in_file && !xml.atEnd())
          {
            switch(xml.readNext())
            {
            case QXmlStreamReader::StartElement:
            {
              BUTTER_REQUIRE(false,
                             QString("Start element not expected in property XML element at line %1").arg(xml.lineNumber()).toStdString());
            }
            break;
            case QXmlStreamReader::EndElement:
            {
              BUTTER_REQUIRE(xml.name() == constants::file_xmlname(),
                             QString("Mismatched end element at line %1").arg(xml.lineNumber()).toStdString());
              in_file = false;
            }
            break;
            case QXmlStreamReader::Characters:
            {
              content += xml.text();
            }
            break;
            default:
            break;
            }
          }
        }
        else if(xml.name() == constants::property_xmlname())
        {
          BUTTER_REQUIRE(xml.attributes().hasAttribute(constants::label_xmlname()),
                         QString("Property XML element does not have required attribute at line %1").arg(xml.lineNumber()).toStdString() );
          QString & content = this->properties_[ xml.attributes().value(constants::label_xmlname()).toString() ];
          bool in_property = true;
          while(in_property && !xml.atEnd())
          {
            switch(xml.readNext())
            {
            case QXmlStreamReader::StartElement:
            {
              BUTTER_REQUIRE(false,
                             QString("Start element not expected in property XML element at line %1").arg(xml.lineNumber()).toStdString());
            }
            break;
            case QXmlStreamReader::EndElement:
            {
              BUTTER_REQUIRE(xml.name() == constants::property_xmlname(),
                             QString("Mismatched end element at line %1").arg(xml.lineNumber()).toStdString());
              in_property = false;
            }
            break;
            case QXmlStreamReader::Characters:
            {
              content += xml.text();
            }
            break;
            default:
            break;
            }
          }
        }
        else
        {
          BUTTER_REQUIRE(false, QString("Unexpected XML start element "+xml.name().toString()+" at line %1").arg(xml.lineNumber()).toStdString() );
        }
      }
      break;
      case QXmlStreamReader::EndElement:
      {
        if(xml.name() == constants::style_xmlname())
        {
          return;
        }
        else
        {
          BUTTER_REQUIRE(false, QString("Unexpected XML end element "+xml.name().toString()+" at line %1").arg(xml.lineNumber()).toStdString() );
        }
      }
      break;
      case QXmlStreamReader::Characters:
        // ignore text outside expected elements
        break;
      case QXmlStreamReader::Comment:
        // Ignore
        break;
      case QXmlStreamReader::ProcessingInstruction:
        // Ignore
        break;
      default:
        // All others are errors!
        throw std::runtime_error( QString("Unexpected XML element '%1' in stream at line %2").arg(xml.name().toString(), QString("%1").arg(xml.lineNumber())).toStdString() );
        break;
      }
    }
    BUTTER_REQUIRE( found_style,  QString("Unknown style '%1'.").arg(style).toStdString() );
  }
}

//Add the contents of other to the style property map, with any
//existing keys being overwritten by the values in other.
build_style::map_type build_style::extend_properties(const build_style::map_type & other) const {
  map_type p{ this->properties_ };
  for(auto itr = other.begin(); itr != other.end(); ++itr)
  {
    p.insert(itr.key(), itr.value());
  }
  return std::move(p);
}

//Name of all non-root build files.
QString build_style::get_build_file_name() {
  auto itr = this->properties_.find( { constants::build_file_xmlname() } );
  return (itr == this->properties_.end() ? QString() : itr.value());
}

//A map of filenames and content of any special build files to create in the root location
const build_style::map_type& build_style::get_properties() const {
  return this->properties_; }

//Name of the root build file.
QString build_style::get_root_file_name() {
  auto itr = this->properties_.find( { constants::build_file_xmlname() } );
  return (itr == this->properties_.end() ? QString() : itr.value());
}

//A map of filenames and content of any special build files to create in the root location
const build_style::map_type& build_style::get_special_files() const {
  return this->special_files_; }

//A map of filenames and content of any special build files to create in the root location
QMap< QString, text_template > const& build_style::get_templates() const {
  return this->templates_; }

//Adds/Checks the close section.
//
//* Using this method instead of directly accessing the templates is
//recommended as it combines the passed properties to the build
//style properties before instantiating the template.
QString build_style::make_close(const build_style::map_type & properties) const {
  auto itr = this->templates_.find( constants::close_xmllabel() );
  if( itr != this->templates_.end() )
  {
    return itr.value().instantiate( this->extend_properties( properties ) );
  }
  return QString();
}

//Generate a comment suitable for putting in the buildfile.
//
//This uses a template labelled "comment", splitting text into
//separate lines and feeding the template a line at a time. If
//no template exists, then the fall-back is to use 
//constants::comment_text
//
//\param text : text to comment

QString build_style::make_comment_text(const QString & text) const {
  auto itr = this->templates_.find( "comment" );
  if( itr != this->templates_.end() )
  {
    QString Result;
    {
      map_type p;
      QTextOStream os_(&Result);
      int s_ = 0; // start position
      for( int e_ = text.find('\n', s_); e_ > 0; e_ = text.find('\n', s_), s_ = e_ + 1 )
      { 
        p[ "text" ] = text.mid(s_, e_ - s_);
        os_ << itr.value().instantiate( p );
        os_ << "\n";
      }
      if( text.at(text.length() - 1) != '\n' )
      {
        p[ "text" ] = text.mid(s_);
        os_ << itr.value().instantiate( p );
        os_ << "\n";
      }
    }
    return Result;
  }
  else
  {
    return constants::comment_text(text);
  }
}

//Adds/Checks the date section
//
//* Using this method instead of directly accessing the templates is
//recommended as it combines the passed properties to the build
//style properties before instantiating the template.
QString build_style::make_date(const build_style::map_type & properties) const {
  auto itr = this->templates_.find( constants::date_xmllabel() );
  map_type p{ this->extend_properties( properties ) };
  if(p.find("date") == p.end())
  {
    p.insert( QString("date"), QDateTime::currentDateTime().toString() );
  }
  if( itr != this->templates_.end() )
  {
    return itr.value().instantiate( p );
  }
  else
  {
    return make_comment_text( p["date"] );
  }
}

//Instantiate the named template with the given properties and build
//properties.  Returns an empty string if the template
//does not exist.
//
//\param properties : map of replacement properties for the template
//\param tmpl : the name of the template to use
//
//* Using this method instead of directly accessing the templates is
//recommended as it combines the passed properties to the build
//style properties before instantiating the template.

QString build_style::make_element(QString tmpl, const build_style::map_type & properties) const {
  auto itr = this->templates_.find( tmpl );
  if( itr != this->templates_.end() )
  {
    return itr.value().instantiate( this->extend_properties( properties ) );
  }
  return QString();
}

//Marker text indicating end of a compound document section with the given name
//
//This uses a template labelled "end-phrase". If
//no template exists, then the fall-back is to use 
//comment_text( " END:" + name )
QString build_style::make_end_phrase(QString name) const {
  auto itr = this->templates_.find( constants::end_phrase_xmlname() );
  if( itr != this->templates_.end() )
  {
    map_type p{ this->properties_ };
    p.insert( QString("name"), name );
    return itr.value().instantiate( p );
  }
  else
  {
    return make_comment_text( "END:" + name );
  }
}

//Adds/Checks the preamble section
//
//* Using this method instead of directly accessing the templates is
//recommended as it combines the passed properties to the build
//style properties before instantiating the template.
QString build_style::make_preamble(const build_style::map_type & properties) const {
  auto itr = this->templates_.find( constants::preamble_xmllabel() );
  if( itr != this->templates_.end() )
  {
    return itr.value().instantiate( this->extend_properties( properties ) );
  }
  return QString();
}

//Marker text indicating beginning of a compound document section with the  given name
//
//This uses a template labelled "start-phrase". If
//no template exists, then the fall-back is to use 
//comment_text( " START:" + name )
QString build_style::make_start_phrase(QString name) const {
  auto itr = this->templates_.find( constants::start_phrase_xmlname() );
  if( itr != this->templates_.end() )
  {
    map_type p{ this->properties_ };
    p.insert( QString("name"), name );
    return itr.value().instantiate( p );
  }
  else
  {
    return make_comment_text( "START:" + name );
  }
}

//Adds/Checks the version section
//
//* Using this method instead of directly accessing the templates is
//recommended as it combines the passed properties to the build
//style properties before instantiating the template.
QString build_style::make_version(const build_style::map_type & properties) const {
  auto itr = this->templates_.find( constants::version_xmllabel() );
  if( itr != this->templates_.end() )
  {
    return itr.value().instantiate( this->extend_properties( properties ) );
  }
  return QString();
}


} // namespace butter

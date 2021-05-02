
#include "text_template.h"

#include "config.h"
#include "constants.h"
#include <qvector.h>
#include <qtextstream.h>
#include <QXmlStreamReader>
namespace butter {

text_template::text_template()
: sequence_() {}

text_template::~text_template() {}

text_template::text_template(const text_template & source)
: sequence_(source.sequence_) {}

text_template::text_template(text_template && source)
: sequence_(std::move(source.sequence_)) {}

void text_template::swap(text_template & source) 
{
  std::swap(sequence_, source.sequence_);
}

//Deserialize template from XML document.
void text_template::read(QXmlStreamReader & xml) {
  BUTTER_REQUIRE(xml.isStartElement() && (xml.name() == constants::text_template_xmlname()),
                 ("XML reader not at a "+constants::text_template_xmlname()+" element in call to text_template::read.").toStdString());
  if(! this->sequence_.empty())
  {
    this->sequence_.clear();
  }
  {
    QVector< QString > phrases;
    while (!xml.atEnd())
    {
      switch(xml.readNext())
      {
      case QXmlStreamReader::StartElement:
      {
        if(xml.name() == constants::replacement_xmlname())
        {
          BUTTER_REQUIRE(xml.attributes().hasAttribute(constants::label_xmlname()),
                         QString("Replacement XML element does not have required attribute at line %1").arg(xml.lineNumber()).toStdString() );
          this->sequence_.push_back( { REPLACEMENT, xml.attributes().value(constants::label_xmlname()).toString() } );
          if(QXmlStreamReader::EndElement != xml.readNext())
          {
            BUTTER_REQUIRE(false,
                          QString("Replacement XML element should be empty at line %1").arg(xml.lineNumber()).toStdString() );
          }
        }
        else if(xml.name() == constants::optional_xmlname())
        {
          BUTTER_REQUIRE(xml.attributes().hasAttribute(constants::label_xmlname()),
                         QString("Optional section XML element does not have required attribute at line %1").arg(xml.lineNumber()).toStdString() );
          phrases.push_back( xml.attributes().value(constants::label_xmlname()).toString() );
          this->sequence_.push_back( { PHRASE, phrases.back() } );
        }
        else
        {
          BUTTER_REQUIRE(false, QString("Unexpected XML start element "+xml.name().toString()+" at line %1").arg(xml.lineNumber()).toStdString() );
        }
      }
      break;
      case QXmlStreamReader::EndElement:
      {
        if(xml.name() == constants::replacement_xmlname())
        {
          BUTTER_REQUIRE(false,
                         QString("Misplaced end to replacement XML element at line %1").arg(xml.lineNumber()).toStdString() );
        }
        else if(xml.name() == constants::text_template_xmlname())
        {
          // At end of this template
          BUTTER_REQUIRE(phrases.empty(), "Incorrectly nested optional sections.");
          return;
        }
        else if(xml.name() == constants::optional_xmlname())
        {
          this->sequence_.push_back( { PHRASE, phrases.back() } );
          phrases.pop_back();
        }
        else
        {
          BUTTER_REQUIRE(false, QString("Unexpected XML end element "+xml.name().toString()+" at line %1").arg(xml.lineNumber()).toStdString() );
        }
      }
      break;
      case QXmlStreamReader::Characters:
      {
        this->sequence_.push_back( { TEXT, xml.text().toString() } );
      }
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
    BUTTER_REQUIRE(phrases.empty(), "Incorrectly nested optional sections.");
  }
}

text_template::const_iterator text_template::begin() const {
  return this->sequence_.begin();
}

bool text_template::empty() const {
  return this->sequence_.empty();
}

text_template::const_iterator text_template::end() const {
  return this->sequence_.end();
}

//Instantiate the template from the data in the given dictionary.
QString text_template::instantiate(const text_template::map_type & properties) const {
  QVector< QString > result;
  result.push_back(QString());
  if(! this->sequence_.empty())
  {
    QVector< QString > phrases;
    QVector< bool > discard_phrase;
    QTextOStream os(&result.back());
    for( const template_entry & entry : this->sequence_ )
    {
      switch(entry.type_)
      {
      case entry_type::TEXT:
        os << entry.content_;
        break;
      case entry_type::REPLACEMENT:
      {
        auto itr{ properties.find(entry.content_) };
        if( itr != properties.end() )
        {
          if(! discard_phrase.empty()) // in phrase, indicate value found
          {
            discard_phrase.back() = false;
          }
          os << itr.value();
        }
      }
      break;
      case entry_type::PHRASE:
      {
        if(! phrases.empty() and phrases.back() == entry.content_)
        {
          // end of optional section
          os.flush();
          QString tmp{ result.back() };
          result.pop_back();
          os.setString(&result.back());
          if(! discard_phrase.back())
          {
            os << tmp;
          }
          phrases.pop_back();
          discard_phrase.pop_back();
        }
        else
        {
          // beginning of optional section
          result.push_back(QString());
          os.setString(&result.back());
          phrases.push_back(entry.content_);
          discard_phrase.push_back(true);
        }
      }
      break;
      }
    }
    BUTTER_REQUIRE(phrases.empty(), "Incorrectly nested optional sections.");
  }
  return result.front();
}

int text_template::size() const {
  return this->sequence_.size();
}

void text_template::push_back(const text_template::template_entry & v) {
  this->sequence_.push_back(v);
}


} // namespace butter

#ifndef BUTTER_TEXT_TEMPLATE_H
#define BUTTER_TEXT_TEMPLATE_H


#include <qstring.h>
#include <qvector.h>

#include <qmap.h>
class QXmlStreamReader;
namespace butter {

class text_template {
  public:
    typedef QMap< QString, QString > map_type;

    enum entry_type {
      TEXT = 0,//Plain text inserted as-is when instantiated
      REPLACEMENT = 1,//A label to search for in a property dictionary.
      PHRASE = 2//Indicate the beginning/end of an optional section. (Any text is used to match the corresponding end.)
      //
      //A phrase is omitted if none of the replacement texts inside the phrase have a match.

    };


  protected:
    struct template_entry {
        //The type of entry
        entry_type type_;

        //Any textual/label content
        QString content_;

    };
    

  public:
    typedef QVector< template_entry >::const_iterator const_iterator;


  private:
    QVector<template_entry> sequence_;


  public:
    text_template();

    ~text_template();

    text_template(const text_template & source);

    text_template(text_template && source);

    void swap(text_template & source);

    text_template & operator=(text_template source)
    {
      this->swap(source); return *this; 
    }

    //Deserialize template from XML document.
    void read(QXmlStreamReader & xml);

    const_iterator begin() const;

    bool empty() const;

    const_iterator end() const;

    //Instantiate the template from the data in the given dictionary.
    QString instantiate(const map_type & properties) const;

    int size() const;

    void push_back(const template_entry & v);

};

} // namespace butter
#endif


#include "bouml/UmlItem.h"

#include "UmlCom.h"
#include "UmlPackage.h"
#include "butter/config.h"
QString UmlItem::kind_as_string() const {
static const char *kind_as_string_[] = {
"relation"
, "attribute"
, "operation"
, "extra class member"
, "class"
, "use case"
, "component"
, "node"
, "artifact"
, "NC relation"
, "class diagram"
, "use case diagram"
, "sequence diagram"
, "collaboration diagram"
, "component diagram"
, "deployment diagram"
, "class view"
, "use case view"
, "component view"
, "deployment view"
, "package"
, "unknown"
};
static const unsigned int size_ (sizeof(kind_as_string_)/sizeof(char*));
const unsigned int our_kind_ (const_cast< UmlItem* >(this)->kind ());
return kind_as_string_[(our_kind_ < 0U || size_ <= our_kind_) ? size_ - 1 : our_kind_];
}

void UmlItem::print(unsigned int a_depth, QTextOStream & a_os) 
{
  BUTTER_ALWAYS (DEBUG, "Programming error: attempt to get debug information in release build");
if (DEBUG)
{
  static const QString title_lbl_ ("[TT] ");
  static const QString watch_lbl_ ("[**] ");
  QString indent_;
  indent_.fill (' ', a_depth);
  a_os << indent_ << title_lbl_ << "Base information\n";
  a_os << indent_ << watch_lbl_ << "name = \"" << name () << "\"\n";
  a_os << indent_ << watch_lbl_ << "depth = \"" << a_depth << "\"\n";
  a_os << indent_ << watch_lbl_ << "stereotype = \"" << stereotype () << "\"\n";

  int kind_ (const_cast< UmlItem& >(*this).kind ());
  a_os << indent_ << watch_lbl_ << "kind = \"" << kind_as_string () << "\" [" << kind_ << "]\n";
  a_os << indent_ << watch_lbl_ << "description = \"" << description () << "\"\n";

  QDict< QCString > properties_ (properties ());

  QDictIterator < QCString > props_ (properties_);

  if (props_.isEmpty ())
  {
    a_os << indent_ << title_lbl_ << "No Properties\n";
  }
  else
  {
    a_os << indent_ << title_lbl_ << "Properties (# = " << props_.count () << ")\n";
    for (; props_.current (); ++props_)
    {
      a_os << indent_ << watch_lbl_ << "[" << props_.currentKey () << "] = \""
	 << *(props_.current ()) << "\"\n";
    }
    a_os << indent_ << title_lbl_ << "End Properties\n";
  }
}

}

bool UmlItem::property_search(QString a_key, QString & a_value) const {
// BUTTER_REQUIRE (! a_key.isEmpty (), "Can not search for an empty key.");
UmlItem *item_ = const_cast< UmlItem* >(this);
const QCString key_ (a_key.utf8 ());
QCString value_;
if (! item_->propertyValue (key_, value_))
{
  while (UmlBasePackage::getProject () != item_)
  {
    item_ = item_->parent ();
    if (item_->propertyValue (key_, value_))
    {
      a_value = value_;
      return true;
    }
  }
  return false;
}
// Property found
a_value = value_;
return true;
}

bool UmlItem::property_value(QString a_key, QString & a_value) const {
// BUTTER_REQUIRE (! a_key.isEmpty (), "Can not search for an empty key.");
QCString value_;
if (const_cast< UmlItem* >(this)->propertyValue (a_key.utf8 (), value_))
{
  a_value = value_;
  return true;
}
return false;

}

 UmlItem::~UmlItem() {
}



#include "bouml/UmlItem.h"

#include "UmlCom.h"
#include "UmlPackage.h"
#ifdef DEBUG
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
#endif

#ifdef DEBUG
void UmlItem::print(unsigned int a_depth, QTextStream & a_os) 
{
static const QString title_lbl__ ("[TT] ");
static const QString watch_lbl__ ("[**] ");
QString indent__;
indent__.fill (' ', a_depth);
a_os << indent__ << title_lbl__ << "Base information\n";
a_os << indent__ << watch_lbl__ << "name = \"" << name () << "\"\n";
a_os << indent__ << watch_lbl__ << "depth = \"" << a_depth << "\"\n";
a_os << indent__ << watch_lbl__ << "stereotype = \"" << stereotype () << "\"\n";

int kind_ (const_cast< UmlItem& >(*this).kind ());
a_os << indent__ << watch_lbl__ << "kind = \"" << kind_as_string () << "\" [" << kind_ << "]\n";
a_os << indent__ << watch_lbl__ << "description = \"" << description () << "\"\n";

QDict< QCString > l_properties (properties ());

QDictIterator < QCString > l_props (l_properties);

if (l_props.isEmpty ())
{
  a_os << indent__ << title_lbl__ << "No Properties\n";
}
else
{
  a_os << indent__ << title_lbl__ << "Properties (# = " << l_props.count () << ")\n";
  for (; l_props.current (); ++l_props)
  {
    a_os << indent__ << watch_lbl__ << "[" << l_props.currentKey () << "] = \"" << *(l_props.current ()) << "\"\n";
  }
  a_os << indent__ << title_lbl__ << "End Properties\n";
}

}
#endif

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


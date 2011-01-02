#ifndef _UMLITEM_H
#define _UMLITEM_H


#include "bouml/UmlBaseItem.h"
#include <qcstring.h>
#include <qstring.h>
#include <qtextstream.h>

/**
 *  This class is a mother class for all the other Uml* classes, this
 *  allows to generalize their management, declare virtual operations etc ...
 * 
 *  You can modify it as you want (except the constructor)
 */
class UmlItem : public UmlBaseItem {
  public:
    /**
     *  returns the description
     */
    const QCString & description() const
    {
      return const_cast< UmlItem& >(*this).UmlBaseItem::description ();
    }
#ifdef DEBUG
    /**
 * Convert a 'kind' into a string
 * 
 * (Only in non NO_LOG builds)
 */
QString kind_as_string() const;
#endif
    QCString name() const
    {
      return const_cast< UmlItem& >(*this).UmlBaseItem::name ();
    }

#ifdef DEBUG
    /**
 * Print information about a_item.
 * 
 * (Only in non NO_LOG  builds)
 */
void print(unsigned int a_depth, QTextStream & a_os);
#endif
    /**
     * Search from a_item to the Project object for the property with a_key. Sets
     * a_value and returns if such a property is found.
     * 
     * \pre not a_key.empty
     */
    bool property_search(QString a_key, QString & a_value) const;

    /**
     * Query a_item for property with a_key. Sets a_value and returns true if such a property is found.
     * 
     * \pre not a_key.empty
     */
    bool property_value(QString a_key, QString & a_value) const;

    /**
     *  returns the stereotype
     */
    QCString stereotype() const
    {
      return const_cast< UmlItem& >(*this).UmlBaseItem::stereotype ();
    }

    UmlItem(void * id, const QCString & n) : UmlBaseItem(id, n) {};

    virtual ~UmlItem();

};

#endif

#ifndef _MYSQLSETTINGS_H
#define _MYSQLSETTINGS_H

#ifdef WITHMYSQL


#include <qbytearray.h>
#include "bouml/UmlSettings.h"

/**
 *  This class manages settings concerning MYSQL, configured through
 *  the 'Generation settings' dialog.
 * 
 *  This class may be defined as a 'singleton', but I prefer to use static
 *  members allowing to just write 'MysqlSettings::member' rather than
 *  'MysqlSettings::instance()->member' or other long sentence like this.
 */
class MysqlSettings : public UmlSettings {
  public:
    /**
     *  returns TRUE when the created MySql objects are initialized
     *  with the default definition
     */
    static bool useDefaults();

    /**
     *  if y is TRUE the future created MySql objects will be initialized
     *  with the default definition
     * 
     *  On error : return FALSE in C++, produce a RuntimeException in Java
     */
    static bool set_UseDefaults(bool y);

    /**
     *  return the 'root' directory
     */
    static const QByteArray & rootDir();

    /**
     *  set the 'root' directory
     * 
     *  On error : return FALSE in C++, produce a RuntimeException in Java
     */
    static bool set_RootDir(QByteArray v);

    /**
     *  returns the default source file content
     */
    static const QByteArray & sourceContent();

    /**
     *  set the default source file content
     * 
     *  On error : return FALSE in C++, produce a RuntimeException in Java
     */
    static bool set_SourceContent(QByteArray v);

    /**
     *  returns the extension of the file produced by the MYSQL code generator
     */
    static const QByteArray & sourceExtension();

    /**
     *  set the extension of the file produced by the MYSQL code generator
     * 
     *  On error : return FALSE in C++, produce a RuntimeException in Java
     */
    static bool set_SourceExtension(QByteArray v);

    /**
     *  returns the default definition of a table
     */
    static const QByteArray & tableDecl();

    /**
     *  set the default definition of a table
     * 
     *  On error : return FALSE in C++, produce a RuntimeException in Java
     */
    static bool set_TableDecl(QByteArray v);

    /**
     *  returns the default definition of a column
     */
    static const QByteArray & columnDecl();

    /**
     *  set the default definition of a column
     * 
     *  On error : return FALSE in C++, produce a RuntimeException in Java
     */
    static bool set_ColumnDecl(QByteArray v);

    /**
     *  returns the default definition of a key
     */
    static const QByteArray & keyDecl();

    /**
     *  set the default definition of a key
     * 
     *  On error : return FALSE in C++, produce a RuntimeException in Java
     */
    static bool set_KeyDecl(QByteArray v);


  private:
    /**
     *  never called !
     */
    MysqlSettings() {};

    static bool _defined;

    static QByteArray _root;

    static QByteArray _table_def;

    static QByteArray _column_def;

    static QByteArray _key_def;

    static QByteArray _src_content;

    static QByteArray _ext;


  protected:
    static void read_();

    static void read_if_needed_();

};

#endif

#endif

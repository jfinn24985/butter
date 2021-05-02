#ifdef WITHMYSQL


#include "bouml/MysqlSettings.h"

#include "UmlCom.h"
#include "MysqlSettingsCmd.h"
/**
 *  returns TRUE when the created MySql objects are initialized
 *  with the default definition
 */
bool MysqlSettings::useDefaults()
{
    UmlCom::send_cmd(mysqlSettingsCmd, getMysqlUseDefaultsCmd);
    return UmlCom::read_bool();
}

/**
 *  if y is TRUE the future created MySql objects will be initialized
 *  with the default definition
 * 
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool MysqlSettings::set_UseDefaults(bool y)
{
    UmlCom::send_cmd(mysqlSettingsCmd, setMysqlUseDefaultsCmd, (char) y);
    return UmlCom::read_bool();
}

/**
 *  return the 'root' directory
 */
const QByteArray & MysqlSettings::rootDir()
{
    read_if_needed_();
  
    return _root;
}

/**
 *  set the 'root' directory
 * 
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool MysqlSettings::set_RootDir(QByteArray v)
{
    UmlCom::send_cmd(mysqlSettingsCmd, setMysqlRootDirCmd, v);
    if (UmlCom::read_bool()) {
      _root = v;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  returns the default source file content
 */
const QByteArray & MysqlSettings::sourceContent()
{
    read_if_needed_();
  
    return _src_content;
}

/**
 *  set the default source file content
 * 
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool MysqlSettings::set_SourceContent(QByteArray v)
{
    UmlCom::send_cmd(mysqlSettingsCmd, setMysqlSourceContentCmd, v);
    if (UmlCom::read_bool()) {
      _src_content = v;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  returns the extension of the file produced by the MYSQL code generator
 */
const QByteArray & MysqlSettings::sourceExtension()
{
    read_if_needed_();
  
    return _ext;
}

/**
 *  set the extension of the file produced by the MYSQL code generator
 * 
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool MysqlSettings::set_SourceExtension(QByteArray v)
{
    UmlCom::send_cmd(mysqlSettingsCmd, setMysqlSourceExtensionCmd, v);
    if (UmlCom::read_bool()) {
      _ext = v;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  returns the default definition of a table
 */
const QByteArray & MysqlSettings::tableDecl()
{
    read_if_needed_();
  
    return _table_def;
}

/**
 *  set the default definition of a table
 * 
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool MysqlSettings::set_TableDecl(QByteArray v)
{
    UmlCom::send_cmd(mysqlSettingsCmd, setMysqlTableDeclCmd, v);
    if (UmlCom::read_bool()) {
      _table_def = v;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  returns the default definition of a column
 */
const QByteArray & MysqlSettings::columnDecl()
{
    read_if_needed_();
  
    return _column_def;
}

/**
 *  set the default definition of a column
 * 
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool MysqlSettings::set_ColumnDecl(QByteArray v)
{
    UmlCom::send_cmd(mysqlSettingsCmd, setMysqlColumnDeclCmd, v);
    if (UmlCom::read_bool()) {
      _column_def = v;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  returns the default definition of a key
 */
const QByteArray & MysqlSettings::keyDecl()
{
    read_if_needed_();
  
    return _key_def;
}

/**
 *  set the default definition of a key
 * 
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool MysqlSettings::set_KeyDecl(QByteArray v)
{
    UmlCom::send_cmd(mysqlSettingsCmd, setMysqlKeyDeclCmd, v);
    if (UmlCom::read_bool()) {
      _key_def = v;
      return TRUE;
    }
    else
      return FALSE;
}

bool MysqlSettings::_defined;

QByteArray MysqlSettings::_root;

QByteArray MysqlSettings::_table_def;

QByteArray MysqlSettings::_column_def;

QByteArray MysqlSettings::_key_def;

QByteArray MysqlSettings::_src_content;

QByteArray MysqlSettings::_ext;

void MysqlSettings::read_()
{
    _root = UmlCom::read_string();
    
    _src_content = UmlCom::read_string();
    _ext = UmlCom::read_string();
  
    _table_def = UmlCom::read_string();
    _column_def = UmlCom::read_string();
    _key_def = UmlCom::read_string();
}

void MysqlSettings::read_if_needed_()
{
    UmlSettings::read_if_needed_();
    if (!_defined) {
      UmlCom::send_cmd(mysqlSettingsCmd, getMysqlSettingsCmd);
      read_();
      _defined = TRUE;
    }
}


#endif

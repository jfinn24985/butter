#ifdef WITHPYTHON


#include "bouml/PythonSettings.h"

#include "UmlCom.h"
#include "PythonSettingsCmd.h"
#include "UmlStereotype.h"
/**
 *  return if classes follow Python 2.2 by default
 */
bool PythonSettings::isPython_2_2()
{
    read_if_needed_();
    return _2_2;
}

/**
 *  set if classes follow Python 2.2 by default
 * 
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool PythonSettings::set_IsPython_2_2(bool y)
{
    UmlCom::send_cmd(pythonSettingsCmd, setPython22Cmd, (char) y);
    if (UmlCom::read_bool()) {
      _2_2 = y;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  returns if operations follow Python 3 (pep-3107)
 */
bool PythonSettings::isPython_3_operation()
{
    read_if_needed_();
  
    return _operation_3;
}

/**
 *  set if operations follow Python 3 (pep-3107)
 * 
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool PythonSettings::set_IsPython_3_operation(bool y)
{
    UmlCom::send_cmd(pythonSettingsCmd, setPython3OperationCmd, y);
    if (UmlCom::read_bool()) {
      _operation_3 = y;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  return the  default indent step
 */
const QByteArray & PythonSettings::indentStep()
{
    read_if_needed_();
    return _indent_step;
}

/**
 *  set default indent step
 * 
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool PythonSettings::set_IndentStep(char * v)
{
    UmlCom::send_cmd(pythonSettingsCmd, setPythonIndentStepCmd, v);
    if (UmlCom::read_bool()) {
      _indent_step = v;
      return TRUE;
    }
    else
      return FALSE;
}

bool PythonSettings::useDefaults()
{
  UmlCom::send_cmd(pythonSettingsCmd, getPythonUseDefaultsCmd);
  return UmlCom::read_bool();
}

bool PythonSettings::set_UseDefaults(bool y)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonUseDefaultsCmd, (char) y);
  return UmlCom::read_bool();
}

QByteArray PythonSettings::relationAttributeStereotype(const QByteArray & s)
{
  read_if_needed_();
  
  UmlStereotype * b = UmlSettings::_map_relation_attribute_stereotypes.find(s);
  
  return (b) ? b->python : s;
}

bool PythonSettings::set_RelationAttributeStereotype(QByteArray s, QByteArray v)
{
  read_if_needed_();
  UmlCom::send_cmd(pythonSettingsCmd, setPythonRelationAttributeStereotypeCmd, s, v);
  if (UmlCom::read_bool()) {
    UmlStereotype * st = UmlSettings::_map_relation_attribute_stereotypes.find(s);

    if (st == 0)
      st = UmlSettings::add_rel_attr_stereotype(s);
    st->python = v;
    
    return TRUE;
  }
  else
    return FALSE;
}

QByteArray PythonSettings::relationAttributeUmlStereotype(const QByteArray & s)
{
  read_if_needed_();
  
  return UmlSettings::uml_rel_attr_stereotype(s, &UmlStereotype::python);
}

QByteArray PythonSettings::classStereotype(const QByteArray & s)
{
  read_if_needed_();
  
  UmlStereotype * b = UmlSettings::_map_class_stereotypes.find(s);
  
  return (b) ? b->python : s;
}

bool PythonSettings::set_ClassStereotype(QByteArray s, QByteArray v)
{
  read_if_needed_();
  UmlCom::send_cmd(pythonSettingsCmd, setPythonClassStereotypeCmd, s, v);
  if (UmlCom::read_bool()) {
    UmlStereotype * st = UmlSettings::_map_class_stereotypes.find(s);

    if (st == 0)
      st = UmlSettings::add_class_stereotype(s);
    st->python = v;
    
    return TRUE;
  }
  else
    return FALSE;
}

QByteArray PythonSettings::classUmlStereotype(const QByteArray & s)
{
  read_if_needed_();
  
  return UmlSettings::uml_class_stereotype(s, &UmlStereotype::python);
}

QByteArray PythonSettings::get_import(const QByteArray & s)
{
  read_if_needed_();
  
  QByteArray * r = _map_imports[s];
  
  return (r) ? *r : QByteArray(0);
}

bool PythonSettings::set_Import(QByteArray s, QByteArray v)
{
  read_if_needed_();
  UmlCom::send_cmd(pythonSettingsCmd, setPythonImportCmd, s, v);
  if (UmlCom::read_bool()) {
    QByteArray * r = _map_imports.take(s);
    
    if (!v.isEmpty())
      _map_imports.insert(s, new QByteArray(v));
    if (r)
      delete r;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PythonSettings::rootDir()
{
  read_if_needed_();
  
  return _root;
}

bool PythonSettings::set_RootDir(QByteArray v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonRootdirCmd, v);
  if (UmlCom::read_bool()) {
    _root = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PythonSettings::sourceContent()
{
  read_if_needed_();
  
  return _src_content;
}

bool PythonSettings::set_SourceContent(QByteArray v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonSourceContentCmd, v);
  if (UmlCom::read_bool()) {
    _src_content = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PythonSettings::sourceExtension()
{
  read_if_needed_();
  
  return _ext; 
}

bool PythonSettings::set_SourceExtension(QByteArray v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonSourceExtensionCmd, v);
  if (UmlCom::read_bool()) {
    _ext = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PythonSettings::classDecl()
{
  read_if_needed_();
  
  return _class_decl;
}

bool PythonSettings::set_ClassDecl(QByteArray v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonClassDeclCmd, v);
  if (UmlCom::read_bool()) {
    _class_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PythonSettings::externalClassDecl()
{
  read_if_needed_();
  
  return _external_class_decl;
}

bool PythonSettings::set_ExternalClassDecl(QByteArray v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonExternalClassDeclCmd, v);
  if (UmlCom::read_bool()) {
    _external_class_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

QByteArray PythonSettings::enumDecl()
{
  read_if_needed_();
  
  return _enum_decl;
}

bool PythonSettings::set_EnumDecl(QByteArray v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonEnumDeclCmd, v);
  if (UmlCom::read_bool()) {
    _enum_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PythonSettings::attributeDecl(const char * multiplicity)
{
  read_if_needed_();

  return _attr_decl[mult_column(multiplicity)];
}

bool PythonSettings::set_AttributeDecl(const char * multiplicity, QByteArray v)
{
  read_if_needed_();
  UmlCom::send_cmd(pythonSettingsCmd, setPythonAttributeDeclCmd, multiplicity, v);
  if (UmlCom::read_bool()) {
    _attr_decl[mult_column(multiplicity)] = v;
    return TRUE;
  }
  else
    return FALSE;
}

QByteArray PythonSettings::enumItemDecl()
{
  read_if_needed_();
  
  return _enum_item_decl;
}

bool PythonSettings::set_EnumItemDecl(QByteArray v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonEnumItemDeclCmd, v);
  if (UmlCom::read_bool()) {
    _enum_item_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PythonSettings::relationDecl(bool composition, const char * multiplicity)
{
  read_if_needed_();
  
  return _rel_decl[(composition) ? 1 : 0][mult_column(multiplicity)];
}

bool PythonSettings::set_RelationDecl(bool composition, const char * multiplicity, const char * v)
{
  read_if_needed_();
  UmlCom::send_cmd(pythonSettingsCmd, setPythonRelationDeclCmd, composition, multiplicity, v);
  if (UmlCom::read_bool()) {
    _rel_decl[(composition) ? 1 : 0][mult_column(multiplicity)] = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PythonSettings::operationDef()
{
  read_if_needed_();
  
  return _oper_def;
}

bool PythonSettings::set_OperationDef(QByteArray v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonOperationDefCmd, v);
  if (UmlCom::read_bool()) {
    _oper_def = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PythonSettings::initOperationDef()
{
  read_if_needed_();

  return _initoper_def;
}

bool PythonSettings::set_InitOperationDef(QByteArray v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonInitOperationDefCmd, v);
  if (UmlCom::read_bool()) {
    _initoper_def = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PythonSettings::getName()
{
  read_if_needed_();
  
  return _get_name;
}

bool PythonSettings::set_GetName(QByteArray v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonGetNameCmd, v);
  if (UmlCom::read_bool()) {
    _get_name = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PythonSettings::setName()
{
  read_if_needed_();
  
  return _set_name;
}

bool PythonSettings::set_SetName(QByteArray v)
{
  UmlCom::send_cmd(pythonSettingsCmd, setPythonSetNameCmd, v);
  if (UmlCom::read_bool()) {
    _set_name = v;
    return TRUE;
  }
  else
    return FALSE;
}

unsigned PythonSettings::mult_column(const QByteArray & mult)
{
    return (mult.isEmpty() || (mult == "1")) ? 0 : 1;
}

bool PythonSettings::_defined;

bool PythonSettings::_2_2;

bool PythonSettings::_operation_3;

QByteArray PythonSettings::_indent_step;

QByteArray PythonSettings::_root;

QByteArray PythonSettings::_class_decl;

QByteArray PythonSettings::_external_class_decl;

QByteArray PythonSettings::_enum_decl;

QByteArray PythonSettings::_attr_decl[2/*multiplicity*/];

QByteArray PythonSettings::_enum_item_decl;

QByteArray PythonSettings::_rel_decl[2/*relation kind*/][2/*multiplicity*/];

QByteArray PythonSettings::_oper_def;

QByteArray PythonSettings::_initoper_def;

QByteArray PythonSettings::_get_name;

QByteArray PythonSettings::_set_name;

QByteArray PythonSettings::_src_content;

QByteArray PythonSettings::_ext;

Q3Dict<QByteArray> PythonSettings::_map_imports;

void PythonSettings::read_()
{
    _2_2 = UmlCom::read_bool();
    
    _indent_step = UmlCom::read_string();
    
    _root = UmlCom::read_string();
    
    unsigned n;
    unsigned index;
    
    n = UmlCom::read_unsigned();
    
    for (index = 0; index != n; index += 1)
      UmlSettings::_relation_attribute_stereotypes[index].python = UmlCom::read_string();
    
    n = UmlCom::read_unsigned();
    
    for (index = 0; index != n; index += 1)
      UmlSettings::_class_stereotypes[index].python = UmlCom::read_string();
    
    n = UmlCom::read_unsigned();
    _map_imports.clear();
    if (n > _map_imports.size())
      _map_imports.resize(n);
    
    for (index = 0; index != n; index += 1) {
      QByteArray t = UmlCom::read_string();
      QByteArray i = UmlCom::read_string();
      
      _map_imports.insert(t, new QByteArray(i));
    }
    
    _src_content = UmlCom::read_string();
    _ext = UmlCom::read_string();
  
    _class_decl = UmlCom::read_string();
    _external_class_decl = UmlCom::read_string();
    _enum_decl = UmlCom::read_string();
    _attr_decl[0] = UmlCom::read_string();
    _attr_decl[0] = UmlCom::read_string();
    _enum_item_decl = UmlCom::read_string();
    _rel_decl[0][0] = UmlCom::read_string();
    _rel_decl[0][1] = UmlCom::read_string();
    _rel_decl[1][0] = UmlCom::read_string();
    _rel_decl[1][1] = UmlCom::read_string();
    _oper_def = UmlCom::read_string();
    _get_name = UmlCom::read_string();
    _set_name = UmlCom::read_string();
    _initoper_def = UmlCom::read_string();
    _operation_3 = UmlCom::read_bool();
}

void PythonSettings::read_if_needed_()
{
    UmlSettings::read_if_needed_();
    if (!_defined) {
      UmlCom::send_cmd(pythonSettingsCmd, getPythonSettingsCmd);
      read_();
      _defined = TRUE;
    }
}


#endif

#ifdef WITHPHP


#include "bouml/PhpSettings.h"

#include "UmlCom.h"
#include "PhpSettingsCmd.h"
#include "JavaSettingsCmd.h"
#include "UmlStereotype.h"
#ifdef WITHJAVA
#include "JavaSettings.h"
#endif
bool PhpSettings::useDefaults()
{
  UmlCom::send_cmd(phpSettingsCmd, getPhpUseDefaultsCmd);
  return UmlCom::read_bool();
}

bool PhpSettings::set_UseDefaults(bool y)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpUseDefaultsCmd, (char) y);
  return UmlCom::read_bool();
}

QByteArray PhpSettings::classStereotype(const QByteArray & s)
{
  read_if_needed_();
  
  UmlStereotype * b = UmlSettings::_map_class_stereotypes.find(s);
  
  return (b) ? b->php : s;
}

bool PhpSettings::set_ClassStereotype(QByteArray s, QByteArray v)
{
  read_if_needed_();
  UmlCom::send_cmd(phpSettingsCmd, setPhpClassStereotypeCmd, s, v);
  if (UmlCom::read_bool()) {
    UmlStereotype * st = UmlSettings::_map_class_stereotypes.find(s);

    if (st == 0)
      st = UmlSettings::add_class_stereotype(s);
    st->php = v;
    
    return TRUE;
  }
  else
    return FALSE;
}

QByteArray PhpSettings::classUmlStereotype(const QByteArray & s)
{
  read_if_needed_();
  
  return UmlSettings::uml_class_stereotype(s, &UmlStereotype::php);
}

const QByteArray & PhpSettings::rootDir()
{
  read_if_needed_();
  
  return _root;
}

bool PhpSettings::set_RootDir(QByteArray v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpRootdirCmd, v);
  if (UmlCom::read_bool()) {
    _root = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PhpSettings::sourceContent()
{
  read_if_needed_();
  
  return _src_content;
}

bool PhpSettings::set_SourceContent(QByteArray v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpSourceContentCmd, v);
  if (UmlCom::read_bool()) {
    _src_content = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PhpSettings::sourceExtension()
{
  read_if_needed_();
  
  return _ext; 
}

bool PhpSettings::set_SourceExtension(QByteArray v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpSourceExtensionCmd, v);
  if (UmlCom::read_bool()) {
    _ext = v;
    return TRUE;
  }
  else
    return FALSE;
}

/**
 *  return the regular expression used to bypass
 *  dir s on reverse/roundtrip
 */
QByteArray PhpSettings::reverseRoundtripDirRegExp()
{
    read_if_needed_();
  
    return _dir_regexp;
}

/**
 *  return if the regular expression used to bypass
 *  dir s on reverse/roundtrip is case sensitive
 */
bool PhpSettings::isReverseRoundtripDirRegExpCaseSensitive()
{
    read_if_needed_();
  
    return _dir_regexp_case_sensitive;
}

/**
 *  set the regular expression used to bypass
 *  dir s on reverse/roundtrip
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool PhpSettings::set_ReverseRoundtripDirRegExp(QByteArray s, bool cs)
{
    UmlCom::send_cmd(phpSettingsCmd, setPhpDirRevFilterCmd, s, cs);
    if (UmlCom::read_bool()) {
      _dir_regexp = s;
      _dir_regexp_case_sensitive = cs;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  return the regular expression used to bypass
 *  file s on reverse/roundtrip
 */
QByteArray PhpSettings::reverseRoundtripFileRegExp()
{
    read_if_needed_();
  
    return _file_regexp;
}

/**
 *  return if the regular expression used to bypass
 *  file s on reverse/roundtrip is case sensitive
 */
bool PhpSettings::isReverseRoundtripFileRegExpCaseSensitive()
{
    read_if_needed_();
  
    return _file_regexp_case_sensitive;
}

/**
 *  set the regular expression used to bypass
 *  file s on reverse/roundtrip
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool PhpSettings::set_ReverseRoundtripFileRegExp(QByteArray s, bool cs)
{
    UmlCom::send_cmd(phpSettingsCmd, setPhpFileRevFilterCmd, s, cs);
    if (UmlCom::read_bool()) {
      _file_regexp = s;
      _file_regexp_case_sensitive = cs;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  indicates to the code generator if the require_once may specify
 *  the path of just the file's name
 */
bool PhpSettings::requireOnceWithPath()
{
    read_if_needed_();
  
    return _req_with_path;
}

/**
 *  to indicates to the code generator if the require_once may specify
 *  the path of just the file's name
 * 
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool PhpSettings::set_RequireOnceWithPath(bool v)
{
    UmlCom::send_cmd(phpSettingsCmd, setPhpRequireOnceWithPathCmd, v);
    if (UmlCom::read_bool()) {
      _req_with_path = v;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  return if a relative path must be used when the path
 *  must be generated in the produced require_once
 */
bool PhpSettings::isRelativePath()
{
    read_if_needed_();
  
    return _is_relative_path;
}

/**
 *  set if a relative path must be used when the path
 *  must be generated in the produced require_once
 * 
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool PhpSettings::set_IsRelativePath(bool v)
{
    UmlCom::send_cmd(phpSettingsCmd, setPhpRelativePathCmd, v);
    if (UmlCom::read_bool()) {
      _is_relative_path = v;
      if (v) _is_root_relative_path = FALSE;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  return if a path relative to the project root must be used
 *  when the path must be generated in the produced require_once
 */
bool PhpSettings::isRootRelativePath()
{
    read_if_needed_();
  
    return _is_root_relative_path;
}

/**
 *  set if a relative to the project root path must be used
 *  when the path must be generated in the produced require_once
 * 
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool PhpSettings::set_IsRootRelativePath(bool v)
{
    UmlCom::send_cmd(phpSettingsCmd, setPhpRootRelativePathCmd, v);
    if (UmlCom::read_bool()) {
      _is_root_relative_path = v;
      if (v) _is_relative_path = FALSE;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  return if the namespace prefix must be
 *  always generated before class's names
 */
bool PhpSettings::isForceNamespacePrefixGeneration()
{
    read_if_needed_();
  
    return _is_force_namespace_gen;
}

/**
 *  set if the namespace prefix must be always generated before class's names
 * 
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool PhpSettings::set_IsForceNamespacePrefixGeneration(bool v)
{
    UmlCom::send_cmd(phpSettingsCmd, setPhpForceNamespaceGenCmd, v);
    if (UmlCom::read_bool()) {
      _is_force_namespace_gen = v;
      return TRUE;
    }
    else
      return FALSE;
}

bool PhpSettings::isGenerateJavadocStyleComment()
{
  read_if_needed_();

  return _is_generate_javadoc_comment;
}

bool PhpSettings::set_IsGenerateJavadocStyleComment(bool v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpJavadocStyleCmd, v);
  if (UmlCom::read_bool()) {
    _is_generate_javadoc_comment = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PhpSettings::classDecl()
{
  read_if_needed_();
  
  return _class_decl;
}

bool PhpSettings::set_ClassDecl(QByteArray v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpClassDeclCmd, v);
  if (UmlCom::read_bool()) {
    _class_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PhpSettings::externalClassDecl()
{
  read_if_needed_();
  
  return _external_class_decl;
}

bool PhpSettings::set_ExternalClassDecl(QByteArray v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpExternalClassDeclCmd, v);
  if (UmlCom::read_bool()) {
    _external_class_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

QByteArray PhpSettings::enumDecl()
{
  read_if_needed_();
  
  return _enum_decl;
}

bool PhpSettings::set_EnumDecl(QByteArray v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpEnumDeclCmd, v);
  if (UmlCom::read_bool()) {
    _enum_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PhpSettings::interfaceDecl()
{
  read_if_needed_();
  
  return _interface_decl;
}

bool PhpSettings::set_InterfaceDecl(QByteArray v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpInterfaceDeclCmd, v);
  if (UmlCom::read_bool()) {
    _interface_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

/**
 *  returns the default definition of a trait
 */
QByteArray PhpSettings::traitDecl()
{
    read_if_needed_();
  
    return _trait_decl;
}

/**
 *  set the default definition of a trait
 * 
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool PhpSettings::set_TraitDecl(QByteArray v)
{
    UmlCom::send_cmd(phpSettingsCmd, setPhpTraitDeclCmd, v);
    if (UmlCom::read_bool()) {
      _trait_decl = v;
      return TRUE;
    }
    else
      return FALSE;
}

const QByteArray & PhpSettings::attributeDecl()
{
  read_if_needed_();

  return _attr_decl;
}

bool PhpSettings::set_AttributeDecl(QByteArray v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpAttributeDeclCmd, v);
  if (UmlCom::read_bool()) {
    _attr_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

QByteArray PhpSettings::enumItemDecl()
{
  read_if_needed_();
  
  return _enum_item_decl;
}

bool PhpSettings::set_EnumItemDecl(QByteArray v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpEnumItemDeclCmd, v);
  if (UmlCom::read_bool()) {
    _enum_item_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PhpSettings::relationDecl()
{
  read_if_needed_();

  return _rel_decl;
}

bool PhpSettings::set_RelationDecl(QByteArray v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpRelationDeclCmd, v);
  if (UmlCom::read_bool()) {
    _rel_decl = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PhpSettings::operationDef()
{
  read_if_needed_();
  
  return _oper_def;
}

bool PhpSettings::set_OperationDef(QByteArray v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpOperationDefCmd, v);
  if (UmlCom::read_bool()) {
    _oper_def = v;
    return TRUE;
  }
  else
    return FALSE;
}

/**
 *  returns TRUE if the parameters are typed by default
 */
bool PhpSettings::isParametersTyped()
{
    read_if_needed_();
  
    return _is_param_typed;
}

/**
 *  to set if parameters are typed by default
 * 
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool PhpSettings::set_IsParametersTyped(bool y)
{
    UmlCom::send_cmd(phpSettingsCmd, setPhpParametersTypedCmd, y);
    if (UmlCom::read_bool()) {
      _is_param_typed = y;
      return TRUE;
    }
    else
      return FALSE;
}

/**
 *  returns TRUE if out/inout parameters are given by reference
 */
bool PhpSettings::isOutInoutParametersByReference()
{
    read_if_needed_();
  
    return _is_out_inout_by_ref;
}

/**
 *  to set if out/inout parameters are given by reference
 * 
 *  On error : return FALSE in C++, produce a RuntimeException in Java
 */
bool PhpSettings::set_IsOutInoutParametersByReference(bool y)
{
    UmlCom::send_cmd(phpSettingsCmd, setPhpOutInoutParametersByReferenceCmd, y);
    if (UmlCom::read_bool()) {
      _is_out_inout_by_ref = y;
      return TRUE;
    }
    else
      return FALSE;
}

aVisibility PhpSettings::getVisibility()
{
#ifdef WITHJAVA
  return JavaSettings::getVisibility();
#else
  read_if_needed_();
  
  return _get_visibility;
#endif
}

bool PhpSettings::set_GetVisibility(aVisibility v)
{
#ifdef WITHJAVA
  return JavaSettings::set_GetVisibility(v);
#else
  UmlCom::send_cmd(phpSettingsCmd, setJavaGetvisibilityCmd, v);
  if (UmlCom::read_bool()) {
    _get_visibility = v;
    return TRUE;
  }
  else
    return FALSE;
#endif
}

const QByteArray & PhpSettings::getName()
{
  read_if_needed_();
  
  return _get_name;
}

bool PhpSettings::set_GetName(QByteArray v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpGetNameCmd, v);
  if (UmlCom::read_bool()) {
    _get_name = v;
    return TRUE;
  }
  else
    return FALSE;
}

const QByteArray & PhpSettings::setName()
{
  read_if_needed_();
  
  return _set_name;
}

bool PhpSettings::set_SetName(QByteArray v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpSetNameCmd, v);
  if (UmlCom::read_bool()) {
    _set_name = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool PhpSettings::isGetFinal()
{
  read_if_needed_();
  
  return _is_get_final;
}

bool PhpSettings::set_IsGetFinal(bool v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpIsGetFinalCmd, v);
  if (UmlCom::read_bool()) {
    _is_get_final = v;
    return TRUE;
  }
  else
    return FALSE;
}

bool PhpSettings::isSetFinal()
{
  read_if_needed_();
  
  return _is_set_final;
}

bool PhpSettings::set_IsSetFinal(bool v)
{
  UmlCom::send_cmd(phpSettingsCmd, setPhpIsSetFinalCmd, v);
  if (UmlCom::read_bool()) {
    _is_set_final = v;
    return TRUE;
  }
  else
    return FALSE;
}

aVisibility PhpSettings::setVisibility()
{
#ifdef WITHJAVA
  return JavaSettings::setVisibility();
#else
  read_if_needed_();
  
  return _set_visibility;
#endif
}

bool PhpSettings::set_SetVisibility(aVisibility v)
{
#ifdef WITHJAVA
  return JavaSettings::set_SetVisibility(v);
#else
  UmlCom::send_cmd(phpSettingsCmd, setJavaSetVisibilityCmd, v);
  if (UmlCom::read_bool()) {
    _set_visibility = v;
    return TRUE;
  }
  else
    return FALSE;
#endif
}

bool PhpSettings::_defined;

QByteArray PhpSettings::_root;

QByteArray PhpSettings::_class_decl;

QByteArray PhpSettings::_external_class_decl;

QByteArray PhpSettings::_enum_decl;

QByteArray PhpSettings::_interface_decl;

QByteArray PhpSettings::_trait_decl;

QByteArray PhpSettings::_attr_decl;

QByteArray PhpSettings::_enum_item_decl;

QByteArray PhpSettings::_rel_decl;

QByteArray PhpSettings::_oper_def;

#ifndef WITHJAVA
    aVisibility PhpSettings::_get_visibility;
#endif

QByteArray PhpSettings::_get_name;

bool PhpSettings::_is_get_final;

#ifndef WITHJAVA
    aVisibility PhpSettings::_set_visibility;
#endif

QByteArray PhpSettings::_set_name;

bool PhpSettings::_is_set_final;

QByteArray PhpSettings::_src_content;

QByteArray PhpSettings::_ext;

QByteArray PhpSettings::_dir_regexp;

bool PhpSettings::_dir_regexp_case_sensitive;

QByteArray PhpSettings::_file_regexp;

bool PhpSettings::_file_regexp_case_sensitive;

bool PhpSettings::_is_generate_javadoc_comment;

bool PhpSettings::_req_with_path;

bool PhpSettings::_is_relative_path;

bool PhpSettings::_is_root_relative_path;

bool PhpSettings::_is_force_namespace_gen;

bool PhpSettings::_is_param_typed;

bool PhpSettings::_is_out_inout_by_ref;

void PhpSettings::read_()
{
  _root = UmlCom::read_string();
  
  unsigned n;
  unsigned index;
  
  n = UmlCom::read_unsigned();
  
  for (index = 0; index != n; index += 1)
    UmlSettings::_class_stereotypes[index].php = UmlCom::read_string();
  
  _src_content = UmlCom::read_string();
  _ext = UmlCom::read_string();

  _class_decl = UmlCom::read_string();
  _external_class_decl = UmlCom::read_string();
  _enum_decl = UmlCom::read_string();
  _interface_decl = UmlCom::read_string();
  _attr_decl = UmlCom::read_string();
  _enum_item_decl = UmlCom::read_string();
  _rel_decl = UmlCom::read_string();
  _oper_def = UmlCom::read_string();
#ifndef WITHJAVA
  _get_visibility =
#endif
    (aVisibility) UmlCom::read_char();
  _get_name = UmlCom::read_string();
  _is_get_final = UmlCom::read_bool();
#ifndef WITHJAVA
  _set_visibility =
#endif
    (aVisibility) UmlCom::read_char();
  _set_name = UmlCom::read_string();
  _is_set_final = UmlCom::read_bool();
  _is_generate_javadoc_comment = UmlCom::read_bool();
  _req_with_path = UmlCom::read_bool();
  _is_relative_path = UmlCom::read_bool();
  _is_root_relative_path = UmlCom::read_bool();

  _dir_regexp = UmlCom::read_string();
  _dir_regexp_case_sensitive = UmlCom::read_bool();

  _file_regexp = UmlCom::read_string();
  _file_regexp_case_sensitive = UmlCom::read_bool();

  _is_force_namespace_gen = UmlCom::read_bool();

  _is_param_typed = UmlCom::read_bool();
  _is_out_inout_by_ref = UmlCom::read_bool();

  _trait_decl = UmlCom::read_string();
}

void PhpSettings::read_if_needed_()
{
  UmlSettings::read_if_needed_();
  if (!_defined) {
    UmlCom::send_cmd(phpSettingsCmd, getPhpSettingsCmd);
    read_();
    _defined = TRUE;
  }
}


#endif

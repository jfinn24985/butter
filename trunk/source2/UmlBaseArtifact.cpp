
#include "UmlBaseArtifact.h"
#include "UmlArtifact.h"
#include "UmlDeploymentView.h"
#include "UmlDeploymentDiagram.h"
#include "UmlClass.h"

#include "UmlCom.h"
UmlArtifact * UmlBaseArtifact::create(UmlDeploymentView * parent, const char * s)
{
  return (UmlArtifact *) parent->create_(anArtifact, s);
}

anItemKind UmlBaseArtifact::kind() {
  return anArtifact;
}

// returns TRUE in case the artifact is locked
bool UmlBaseArtifact::isLocked() {
    read_if_needed_();
  
    return _locked;
}

UmlDeploymentDiagram * UmlBaseArtifact::associatedDiagram() {
  read_if_needed_();
  
  return _assoc_diagram;
}

bool UmlBaseArtifact::set_AssociatedDiagram(UmlDeploymentDiagram * d) {
  UmlCom::send_cmd(_identifier, setAssocDiagramCmd, (d == 0) ? (void *) 0 : ((UmlBaseItem *) d)->_identifier);
  if (UmlCom::read_bool()) {
    _assoc_diagram = d;
    return TRUE;
  }
  else
    return FALSE;
}

const Q3PtrVector<UmlItem> & UmlBaseArtifact::associatedElements() {
  read_if_needed_();

  return _assoc_elems;
}

bool UmlBaseArtifact::addAssociatedClass(UmlClass * cl) {
  UmlCom::send_cmd(_identifier, addAssocClassCmd,
		   ((UmlBaseClass *) cl)->_identifier);
  if (UmlCom::read_bool()) {
    if (_defined) {
      _assoc_elems.resize(_assoc_elems.size() + 1);
      _assoc_elems.insert(_assoc_elems.size() - 1, cl);
    }
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseArtifact::removeAssociatedClass(UmlClass * cl) {
  UmlCom::send_cmd(_identifier, removeAssocClassCmd,
		   ((UmlBaseClass *) cl)->_identifier);
  if (UmlCom::read_bool()) {
    if (_defined) {
      int i = _assoc_elems.findRef(cl);
      
      if (i != -1)
	// normaly always the case
	_assoc_elems.take((unsigned) i);
    }
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseArtifact::set_AssociatedElements(const Q3PtrVector<UmlItem> & l) {
  unsigned n = l.count();
  UmlItem ** v = l.data();
  UmlItem ** vsup = v + n;
  
  for (; v != vsup; v += 1) {
    switch ((*v)->kind()) {
    case aClass:
    case anExtraArtifactDefinition:
      break;
    default:
      return FALSE;
    }
  }

  UmlCom::send_cmd(_identifier, setAssocClassesCmd, l);
  if (UmlCom::read_bool()) {
    if (_defined)
      _assoc_elems = l;
    return TRUE;
  }
  else
    return FALSE;
}

const Q3PtrVector<UmlArtifact> & UmlBaseArtifact::associatedArtifacts() {
  read_if_needed_();
  
  return _associated;
}

bool UmlBaseArtifact::addAssociatedArtifact(UmlArtifact * cp) {
  UmlCom::send_cmd(_identifier, addAssocArtifactCmd, 
		   ((UmlBaseArtifact *) cp)->_identifier);
  if (UmlCom::read_bool()) {
    if (_defined) {
      _associated.resize(_associated.size() + 1);
      _associated.insert(_associated.size() - 1, cp);
    }
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseArtifact::removeAssociatedArtifact(UmlArtifact * cp) {
  UmlCom::send_cmd(_identifier, removeAssocArtifactCmd, 
		   ((UmlBaseArtifact *) cp)->_identifier);
  if (UmlCom::read_bool()) {
    if (_defined) {
      unsigned index = (unsigned) _associated.findRef(cp);
      
      if (((int) index) == -1)
	// theo impossible
	return FALSE;
      
      unsigned last = _associated.size() - 1;
      
      if (index != last)
	_associated.insert(index, _associated[last]);
      
      _associated.resize(last);
    }
    return TRUE;
  }
  else
    return FALSE;
}

bool UmlBaseArtifact::removeAllAssociatedArtifacts() {
  UmlCom::send_cmd(_identifier, removeAllAssocArtifactsCmd);
  if (UmlCom::read_bool()) {  
    _associated.clear();
    return TRUE;
  }
  else
    return FALSE;
}

#ifdef WITHCPP
const QByteArray & UmlBaseArtifact::cppHeader() {
  read_if_needed_();
  
  return _cpp_h;
}

bool UmlBaseArtifact::set_CppHeader(const QByteArray & s) {
  return set_it_(_cpp_h, s, setCppHCmd);
}

const QByteArray & UmlBaseArtifact::cppSource() {
  read_if_needed_();
  
  return _cpp_src;
}

bool UmlBaseArtifact::set_CppSource(const QByteArray & s) {
  return set_it_(_cpp_src, s, setCppSrcCmd);
}
#endif

#ifdef WITHJAVA
const QByteArray & UmlBaseArtifact::javaSource() {
  read_if_needed_();
  
  return _java_src;
}

bool UmlBaseArtifact::set_JavaSource(const QByteArray & s) {
  return set_it_(_java_src, s, setJavaSrcCmd);
}
#endif

#ifdef WITHPHP
const QByteArray & UmlBaseArtifact::phpSource() {
  read_if_needed_();
  
  return _php_src;
}

bool UmlBaseArtifact::set_PhpSource(const QByteArray & s) {
  return set_it_(_php_src, s, setPhpSrcCmd);
}
#endif

#ifdef WITHPYTHON
const QByteArray & UmlBaseArtifact::pythonSource() {
  read_if_needed_();
  
  return _python_src;
}

bool UmlBaseArtifact::set_PythonSource(const QByteArray & s) {
  return set_it_(_python_src, s, setPythonSrcCmd);
}
#endif

#ifdef WITHIDL
const QByteArray & UmlBaseArtifact::idlSource() {
  read_if_needed_();
  
  return _idl_src;
}

bool UmlBaseArtifact::set_IdlSource(const QByteArray & s) {
  return set_it_(_idl_src, s, setIdlSrcCmd);
}
#endif

#ifdef WITHMYSQL
// returns the MySql file definition
const QByteArray & UmlBaseArtifact::mysqlSource() {
    read_if_needed_();
  
    return _mysql_src;
}

// to set the MySql file definition
//
// On error : return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseArtifact::set_MysqlSource(const QByteArray & s) {
    return set_it_(_mysql_src, s, setMysqlSrcCmd);
}
#endif

void UmlBaseArtifact::unload(bool rec, bool del) {
  _assoc_elems.clear();
  _associated.clear();
#ifdef WITHCPP
  _cpp_h = 0;
  _cpp_src = 0;
#endif
#ifdef WITHJAVA
  _java_src = 0;
#endif
#ifdef WITHPHP
  _php_src = 0;
#endif
#ifdef WITHPYTHON
  _python_src = 0;
#endif
#ifdef WITHMYSQL
  _mysql_src = 0;
#endif
#ifdef WITHIDL
  _idl_src = 0;
#endif
  UmlBaseItem::unload(rec, del);
}

void UmlBaseArtifact::read_uml_() {
  _assoc_diagram = (UmlDeploymentDiagram *) UmlBaseItem::read_();
  UmlBaseItem::read_uml_();
  
  unsigned n;
  unsigned index;
  
  n = UmlCom::read_unsigned();
  _assoc_elems.resize(n);
    
  for (index = 0; index != n; index += 1)
    _assoc_elems.insert(index, UmlBaseItem::read_());
  
  n = UmlCom::read_unsigned();
  _associated.resize(n);
    
  for (index = 0; index != n; index += 1)
    _associated.insert(index, (UmlArtifact *) UmlBaseItem::read_());

  _locked = UmlCom::read_bool();
}

#ifdef WITHCPP
void UmlBaseArtifact::read_cpp_() {
  _cpp_h = UmlCom::read_string();
  _cpp_src = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseArtifact::read_java_() {
  _java_src = UmlCom::read_string();
}
#endif

#ifdef WITHPHP
void UmlBaseArtifact::read_php_() {
  _php_src = UmlCom::read_string();
}
#endif

#ifdef WITHPYTHON
//internal, do NOT use it

void UmlBaseArtifact::read_python_() {
    _python_src = UmlCom::read_string();
}
#endif

#ifdef WITHMYSQL
//internal, do NOT use it

void UmlBaseArtifact::read_mysql_() {
    _mysql_src = UmlCom::read_string();
}
#endif

#ifdef WITHIDL
void UmlBaseArtifact::read_idl_() {
  _idl_src = UmlCom::read_string();
}
#endif


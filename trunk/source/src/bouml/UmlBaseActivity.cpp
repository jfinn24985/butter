
#include "UmlCom.h"
#include "bouml/UmlBaseActivity.h"
#include "bouml/UmlActivity.h"
#include "bouml/UmlClassView.h"
#include "bouml/UmlOperation.h"
#include "bouml/UmlActivityDiagram.h"

UmlActivity * UmlBaseActivity::create(UmlClassView * parent, const char * s)
{
  return (UmlActivity *) parent->create_(anActivity, s);
}

anItemKind UmlBaseActivity::kind() {
  return anActivity;
}

const QByteArray & UmlBaseActivity::preCondition() {
  read_if_needed_();
  return _pre_condition;
}

bool UmlBaseActivity::set_PreCondition(const char * v) {
  return set_it_(_pre_condition, v, setUmlEntryBehaviorCmd);
}

const QByteArray & UmlBaseActivity::postCondition() {
  read_if_needed_();
  return _post_condition;
}

bool UmlBaseActivity::set_PostCondition(const char * v) {
  return set_it_(_post_condition, v, setUmlExitBehaviorCmd);
}

#ifdef WITHCPP
const QByteArray & UmlBaseActivity::cppPreCondition() {
  read_if_needed_();
  return _cpp_pre_condition;
}

bool UmlBaseActivity::set_CppPreCondition(const char * v) {
  return set_it_(_cpp_pre_condition, v, setCppEntryBehaviorCmd);
}

const QByteArray & UmlBaseActivity::cppPostCondition() {
  read_if_needed_();
  return _cpp_post_condition;
}

bool UmlBaseActivity::set_CppPostCondition(const char * v) {
  return set_it_(_cpp_post_condition, v, setCppExitBehaviorCmd);
}
#endif

#ifdef WITHJAVA
const QByteArray & UmlBaseActivity::javaPreCondition() {
  read_if_needed_();
  return _java_pre_condition;
}

bool UmlBaseActivity::set_JavaPreCondition(const char * v) {
  return set_it_(_java_pre_condition, v, setJavaEntryBehaviorCmd);
}

const QByteArray & UmlBaseActivity::javaPostCondition() {
  read_if_needed_();
  return _java_post_condition;
}

bool UmlBaseActivity::set_JavaPostCondition(const char * v) {
  return set_it_(_java_post_condition, v, setJavaExitBehaviorCmd);
}
#endif

/**
 *  return the constraint
 */
const QByteArray & UmlBaseActivity::constraint() {
    read_if_needed_();
    return _constraint;
}

/**
 *  set the constraint
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBaseActivity::set_Constraint(const char * v) {
    return set_it_(_constraint, v, setConstraintCmd);
}

bool UmlBaseActivity::isReadOnly() {
  read_if_needed_();
  return _read_only;
}

bool UmlBaseActivity::set_isReadOnly(bool v) {
  return set_it_(_read_only, v, setReadOnlyCmd);
}

bool UmlBaseActivity::isSingleExecution() {
  read_if_needed_();
  return _single_execution;
}

bool UmlBaseActivity::set_isSingleExecution(bool v) {
  return set_it_(_single_execution, v, setSingleExecutionCmd);
}

/**
 *  return the  indicate if the class is active
 */
bool UmlBaseActivity::isActive() {
    read_if_needed_();
    return _active;
}

/**
 *  set if the class is active
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBaseActivity::set_isActive(bool v) {
    return set_it_(_active, v, setActiveCmd);
}

/**
 *  return the specification
 */
UmlOperation * UmlBaseActivity::specification() {
    read_if_needed_();
    return _specification;
}

/**
 *  set the specification
 * 
 *  On error return FALSE in C++, produce a RuntimeException in Java
 */
bool UmlBaseActivity::set_Specification(UmlOperation * v) {
    UmlCom::send_cmd(_identifier, setDefCmd, (v == 0) ? (void *) v : ((UmlBaseItem *) v)->_identifier);
    if (UmlCom::read_bool()) {
      _specification = v;
      return TRUE;
    }
    else
      return FALSE;
}

UmlActivityDiagram * UmlBaseActivity::associatedDiagram() {
  read_if_needed_();

  return _assoc_diagram;
}

bool UmlBaseActivity::set_AssociatedDiagram(UmlActivityDiagram * d) {
  UmlCom::send_cmd(_identifier, setAssocDiagramCmd, (d == 0) ? (void *) 0 : ((UmlBaseItem *) d)->_identifier);
  if (UmlCom::read_bool()) {
    _assoc_diagram = d;
    return TRUE;
  }
  else
    return FALSE;
}

void UmlBaseActivity::unload(bool rec, bool del) {
  _pre_condition = 0;
  _post_condition = 0;
#ifdef WITHCPP
  _cpp_pre_condition = 0;
  _cpp_post_condition = 0;
#endif
#ifdef WITHJAVA
  _java_pre_condition = 0;
  _java_post_condition = 0;
#endif
  UmlBaseItem::unload(rec, del);
  _constraint = 0;
}

void UmlBaseActivity::read_uml_() {
  _assoc_diagram = (UmlActivityDiagram *) UmlBaseItem::read_();
  UmlBaseItem::read_uml_();
  _pre_condition = UmlCom::read_string();
  _post_condition = UmlCom::read_string();
  _read_only = UmlCom::read_bool();
  _single_execution = UmlCom::read_bool();
  _specification = (UmlOperation *) UmlBaseItem::read_();
  _active = UmlCom::read_bool();
  _constraint = UmlCom::read_string();
}

#ifdef WITHCPP
void UmlBaseActivity::read_cpp_() {
  _cpp_pre_condition = UmlCom::read_string();
  _cpp_post_condition = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseActivity::read_java_() {
  _java_pre_condition = UmlCom::read_string();
  _java_post_condition = UmlCom::read_string();
}
#endif


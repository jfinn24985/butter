
#include "UmlCom.h"
#include "UmlBaseActivityActionClasses.h"
#include "UmlActivityActionClasses.h"
#include "UmlItem.h"
#include "UmlOperation.h"

UmlSendObjectAction * UmlBaseSendObjectAction::create(UmlItem * parent, const char * s)
{
  return (UmlSendObjectAction *) parent->create_(aSendObjectAction, s);
}

anItemKind UmlBaseSendObjectAction::kind() {
  return aSendObjectAction;
}

UmlUnmarshallAction * UmlBaseUnmarshallAction::create(UmlItem * parent, const char * s)
{
  return (UmlUnmarshallAction *) parent->create_(anUnmarshallAction, s);
}

anItemKind UmlBaseUnmarshallAction::kind() {
  return anUnmarshallAction;
}

// return the classifier
const QByteArray & UmlBaseUnmarshallAction::classifier() {
    read_if_needed_();
    return _classifier;
}

// set the classifier
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseUnmarshallAction::set_Classifier(const QByteArray v) {
    return set_it_(_classifier, v, setUmlActivityCmd);
}

void UmlBaseUnmarshallAction::read_uml_() {
    UmlBaseActivityAction::read_uml_();
    _classifier = UmlCom::read_string();
}

UmlSendSignalAction * UmlBaseSendSignalAction::create(UmlItem * parent, const char * s)
{
  return (UmlSendSignalAction *) parent->create_(aSendSignalAction, s);
}

anItemKind UmlBaseSendSignalAction::kind() {
  return aSendSignalAction;
}

UmlBroadcastSignalAction * UmlBaseBroadcastSignalAction::create(UmlItem * parent, const char * s)
{
  return (UmlBroadcastSignalAction *) parent->create_(aBroadcastSignalAction, s);
}

anItemKind UmlBaseBroadcastSignalAction::kind() {
  return aBroadcastSignalAction;
}

UmlValueSpecificationAction * UmlBaseValueSpecificationAction::create(UmlItem * parent, const char * s)
{
  return (UmlValueSpecificationAction *) parent->create_(aValueSpecificationAction, s);
}

anItemKind UmlBaseValueSpecificationAction::kind() {
  return aValueSpecificationAction;
}

const QByteArray & UmlBaseValueSpecificationAction::value() {
  read_if_needed_();
  return _value;
}

bool UmlBaseValueSpecificationAction::set_Value(const char * v) {
  return set_it_(_value, v, setUmlActivityCmd);
}

#ifdef WITHCPP
const QByteArray & UmlBaseValueSpecificationAction::cppValue() {
  read_if_needed_();
  return _cpp_value;
}

bool UmlBaseValueSpecificationAction::set_CppValue(const char * v) {
  return set_it_(_cpp_value, v, setCppActivityCmd);
}
#endif

#ifdef WITHJAVA
const QByteArray & UmlBaseValueSpecificationAction::javaValue() {
  read_if_needed_();
  return _java_value;
}

bool UmlBaseValueSpecificationAction::set_JavaValue(const char * v) {
  return set_it_(_java_value, v, setJavaActivityCmd);
}
#endif

void UmlBaseValueSpecificationAction::unload(bool rec, bool del) {
  _value = 0;
#ifdef WITHCPP
  _cpp_value = 0;
#endif
#ifdef WITHJAVA
  _java_value = 0;
#endif
  UmlBaseActivityAction::unload(rec, del);
}

void UmlBaseValueSpecificationAction::read_uml_() {
  UmlBaseActivityAction::read_uml_();
  _value = UmlCom::read_string();
}

#ifdef WITHCPP
void UmlBaseValueSpecificationAction::read_cpp_() {
  UmlBaseActivityAction::read_cpp_();
  _cpp_value = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseValueSpecificationAction::read_java_() {
  UmlBaseActivityAction::read_java_();
  _java_value = UmlCom::read_string();
}
#endif

UmlOpaqueAction * UmlBaseOpaqueAction::create(UmlItem * parent, const char * s)
{
  return (UmlOpaqueAction *) parent->create_(anOpaqueAction, s);
}

anItemKind UmlBaseOpaqueAction::kind() {
  return anOpaqueAction;
}

const QByteArray & UmlBaseOpaqueAction::behavior() {
  read_if_needed_();
  return _behavior;
}

bool UmlBaseOpaqueAction::set_Behavior(const char * v) {
  return set_it_(_behavior, v, setUmlActivityCmd);
}

#ifdef WITHCPP
const QByteArray & UmlBaseOpaqueAction::cppBehavior() {
  read_if_needed_();
  return _cpp_behavior;
}

bool UmlBaseOpaqueAction::set_CppBehavior(const char * v) {
  return set_it_(_cpp_behavior, v, setCppActivityCmd);
}
#endif

#ifdef WITHJAVA
const QByteArray & UmlBaseOpaqueAction::javaBehavior() {
  read_if_needed_();
  return _java_behavior;
}

bool UmlBaseOpaqueAction::set_JavaBehavior(const char * v) {
  return set_it_(_java_behavior, v, setJavaActivityCmd);
}
#endif

void UmlBaseOpaqueAction::unload(bool rec, bool del) {
  _behavior = 0;
#ifdef WITHCPP
  _cpp_behavior = 0;
#endif
#ifdef WITHJAVA
  _java_behavior = 0;
#endif
  UmlBaseActivityAction::unload(rec, del);
}

void UmlBaseOpaqueAction::read_uml_() {
  UmlBaseActivityAction::read_uml_();
  _behavior = UmlCom::read_string();
}

#ifdef WITHCPP
void UmlBaseOpaqueAction::read_cpp_() {
  UmlBaseActivityAction::read_cpp_();
  _cpp_behavior = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseOpaqueAction::read_java_() {
  UmlBaseActivityAction::read_java_();
  _java_behavior = UmlCom::read_string();
}
#endif

UmlAcceptEventAction * UmlBaseAcceptEventAction::create(UmlItem * parent, const char * s)
{
  return (UmlAcceptEventAction *) parent->create_(anAcceptEventAction, s);
}

anItemKind UmlBaseAcceptEventAction::kind() {
  return anAcceptEventAction;
}

bool UmlBaseAcceptEventAction::isUnmarshall() {
  read_if_needed_();
  return _unmarshall;
}

bool UmlBaseAcceptEventAction::set_isUnmarshall(bool v) {
  return set_it_(_unmarshall, v, setUnmarshallCmd);
}

bool UmlBaseAcceptEventAction::isTimeEvent() {
  read_if_needed_();
  return _timeevent;
}

bool UmlBaseAcceptEventAction::set_isTimeEvent(bool v) {
  return set_it_(_timeevent, v, setTimeEventCmd);
}

const QByteArray & UmlBaseAcceptEventAction::trigger() {
  read_if_needed_();
  return _trigger;
}

bool UmlBaseAcceptEventAction::set_Trigger(const char * v) {
  return set_it_(_trigger, v, setUmlTriggerCmd);
}

#ifdef WITHCPP
const QByteArray & UmlBaseAcceptEventAction::cppTrigger() {
  read_if_needed_();
  return _cpp_trigger;
}

bool UmlBaseAcceptEventAction::set_CppTrigger(const char * v) {
  return set_it_(_cpp_trigger, v, setCppTriggerCmd);
}
#endif

#ifdef WITHJAVA
const QByteArray & UmlBaseAcceptEventAction::javaTrigger() {
  read_if_needed_();
  return _java_trigger;
}

bool UmlBaseAcceptEventAction::set_JavaTrigger(const char * v) {
  return set_it_(_java_trigger, v, setJavaTriggerCmd);
}
#endif

void UmlBaseAcceptEventAction::unload(bool rec, bool del) {
  _trigger = 0;
#ifdef WITHCPP
  _cpp_trigger = 0;
#endif
#ifdef WITHJAVA
  _java_trigger = 0;
#endif
  UmlBaseActivityAction::unload(rec, del);
}

void UmlBaseAcceptEventAction::read_uml_() {
  UmlBaseActivityAction::read_uml_();
  _unmarshall = UmlCom::read_bool();
  _timeevent = UmlCom::read_bool();
  _trigger = UmlCom::read_string();
}

#ifdef WITHCPP
void UmlBaseAcceptEventAction::read_cpp_() {
  UmlBaseActivityAction::read_cpp_();
  _cpp_trigger = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseAcceptEventAction::read_java_() {
  UmlBaseActivityAction::read_java_();
  _java_trigger = UmlCom::read_string();
}
#endif

UmlCallOperationAction * UmlBaseCallOperationAction::create(UmlItem * parent, const char * s)
{
  return (UmlCallOperationAction *) parent->create_(aCallOperationAction, s);
}

anItemKind UmlBaseCallOperationAction::kind() {
  return aCallOperationAction;
}

bool UmlBaseCallOperationAction::isSynchronous() {
  read_if_needed_();
  return _synchronous;
}

bool UmlBaseCallOperationAction::set_isSynchronous(bool v) {
  return set_it_(_synchronous, v, setFlagCmd);
}

UmlOperation * UmlBaseCallOperationAction::operation() {
  read_if_needed_();
  return _operation;
}

bool UmlBaseCallOperationAction::set_Operation(UmlOperation * v) {
  UmlCom::send_cmd(_identifier, setDefCmd, ((UmlBaseItem *) v)->_identifier);   if (UmlCom::read_bool()) {     _operation = v;     return TRUE;   }   else     return FALSE;
}

void UmlBaseCallOperationAction::read_uml_() {
  UmlBaseActivityAction::read_uml_();
  _synchronous = UmlCom::read_bool();
  _operation = (UmlOperation *) UmlBaseItem::read_();
}

UmlCallBehaviorAction * UmlBaseCallBehaviorAction::create(UmlItem * parent, const char * s)
{
  return (UmlCallBehaviorAction *) parent->create_(aCallBehaviorAction, s);
}

anItemKind UmlBaseCallBehaviorAction::kind() {
  return aCallBehaviorAction;
}

bool UmlBaseCallBehaviorAction::isSynchronous() {
  read_if_needed_();
  return _synchronous;
}

bool UmlBaseCallBehaviorAction::set_isSynchronous(bool v) {
  return set_it_(_synchronous, v, setFlagCmd);
}

UmlItem * UmlBaseCallBehaviorAction::behavior() {
  read_if_needed_();
  return _behavior;
}

bool UmlBaseCallBehaviorAction::set_Behavior(UmlItem * v) {
  UmlCom::send_cmd(_identifier, setDefCmd, ((UmlBaseItem *) v)->_identifier);   if (UmlCom::read_bool()) {     _behavior = v;     return TRUE;   }   else     return FALSE;
}

void UmlBaseCallBehaviorAction::read_uml_() {
  UmlBaseActivityAction::read_uml_();
  _synchronous = UmlCom::read_bool();
  _behavior = UmlBaseItem::read_();
}

UmlClearVariableValueAction * UmlBaseClearVariableValueAction::create(UmlItem * parent, const char * s)
{
  return (UmlClearVariableValueAction *) parent->create_(aClearVariableValueAction, s);
}

anItemKind UmlBaseClearVariableValueAction::kind() {
  return aClearVariableValueAction;
}

UmlReadVariableValueAction * UmlBaseReadVariableValueAction::create(UmlItem * parent, const char * s)
{
  return (UmlReadVariableValueAction *) parent->create_(aReadVariableValueAction, s);
}

anItemKind UmlBaseReadVariableValueAction::kind() {
  return aReadVariableValueAction;
}

UmlAddVariableValueAction * UmlBaseAddVariableValueAction::create(UmlItem * parent, const char * s)
{
  return (UmlAddVariableValueAction *) parent->create_(anAddVariableValueAction, s);
}

anItemKind UmlBaseAddVariableValueAction::kind() {
  return anAddVariableValueAction;
}

bool UmlBaseAddVariableValueAction::isReplaceAll() {
  read_if_needed_();
  return _replace_all;
}

bool UmlBaseAddVariableValueAction::set_isReplaceAll(bool v) {
  return set_it_(_replace_all, v, setFlagCmd);
}

void UmlBaseAddVariableValueAction::read_uml_() {
  UmlBaseAccessVariableValueAction::read_uml_();
  _replace_all = UmlCom::read_bool();
}

UmlRemoveVariableValueAction * UmlBaseRemoveVariableValueAction::create(UmlItem * parent, const char * s)
{
  return (UmlRemoveVariableValueAction *) parent->create_(aRemoveVariableValueAction, s);
}

anItemKind UmlBaseRemoveVariableValueAction::kind() {
  return aRemoveVariableValueAction;
}

bool UmlBaseRemoveVariableValueAction::isRemoveDuplicates() {
  read_if_needed_();
  return _remove_duplicates;
}

bool UmlBaseRemoveVariableValueAction::set_isRemoveDuplicates(bool v) {
  return set_it_(_remove_duplicates, v, setFlagCmd);
}

void UmlBaseRemoveVariableValueAction::read_uml_() {
  UmlBaseAccessVariableValueAction::read_uml_();
  _remove_duplicates = UmlCom::read_bool();
}

//  returns a new accept call action named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlAcceptCallAction * UmlBaseAcceptCallAction::create(UmlItem * parent, const char * s)
{
    return (UmlAcceptCallAction *) parent->create_(anAcceptCallAction, s);
}

// returns the kind of the item
anItemKind UmlBaseAcceptCallAction::kind() {
    return anAcceptCallAction;
}

// return the property 'unmarshall'
bool UmlBaseAcceptCallAction::isUnmarshall() {
    read_if_needed_();
    return _unmarshall;
}

// set the property 'unmarshall'
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAcceptCallAction::set_isUnmarshall(bool v) {
    return set_it_(_unmarshall, v, setUnmarshallCmd);
}

// return the trigger
const QByteArray & UmlBaseAcceptCallAction::trigger() {
    read_if_needed_();
    return _trigger;
}

// set the trigger
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAcceptCallAction::set_Trigger(const char * v) {
    return set_it_(_trigger, v, setUmlTriggerCmd);
}

#ifdef WITHCPP
// return the trigger in C++
const QByteArray & UmlBaseAcceptCallAction::cppTrigger() {
    read_if_needed_();
    return _cpp_trigger;
}

// set the trigger in C++
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAcceptCallAction::set_CppTrigger(const char * v) {
    return set_it_(_cpp_trigger, v, setCppTriggerCmd);
}
#endif

#ifdef WITHJAVA
// return the trigger in Java
const QByteArray & UmlBaseAcceptCallAction::javaTrigger() {
    read_if_needed_();
    return _java_trigger;
}

// set the trigger in Java
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAcceptCallAction::set_JavaTrigger(const char * v) {
    return set_it_(_java_trigger, v, setJavaTriggerCmd);
}
#endif

// to unload the object to free memory, it will be reloaded automatically
// if needed. Recursively done for the sub items if 'rec' is TRUE.
//
// if 'del' is true the sub items are deleted in C++, and removed from the
// internal dictionnary in C++ and Java (to allow it to be garbaged),
// you will have to call Children() to re-access to them
void UmlBaseAcceptCallAction::unload(bool rec, bool del) {
    _trigger = 0;
#ifdef WITHCPP
    _cpp_trigger = 0;
#endif
#ifdef WITHJAVA
    _java_trigger = 0;
#endif
    UmlBaseActivityAction::unload(rec, del);
}

void UmlBaseAcceptCallAction::read_uml_() {
    UmlBaseActivityAction::read_uml_();
    _unmarshall = UmlCom::read_bool();
    _trigger = UmlCom::read_string();
}

#ifdef WITHCPP
void UmlBaseAcceptCallAction::read_cpp_() {
    UmlBaseActivityAction::read_cpp_();
    _cpp_trigger = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseAcceptCallAction::read_java_() {
    UmlBaseActivityAction::read_java_();
    _java_trigger = UmlCom::read_string();
}
#endif

//  returns a new reply action named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlReplyAction * UmlBaseReplyAction::create(UmlItem * parent, const char * s)
{
    return (UmlReplyAction *) parent->create_(aReplyAction, s);
}

// returns the kind of the item
anItemKind UmlBaseReplyAction::kind() {
    return aReplyAction;
}

// return the replyToCall trigger
const QByteArray & UmlBaseReplyAction::replyToCall() {
    read_if_needed_();
    return _trigger;
}

// set the replyToCall trigger
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseReplyAction::set_ReplyToCall(const char * v) {
    return set_it_(_trigger, v, setUmlTriggerCmd);
}

#ifdef WITHCPP
// return the replyToCall trigger in C++
const QByteArray & UmlBaseReplyAction::cppReplyToCall() {
    read_if_needed_();
    return _cpp_trigger;
}

// set the replyToCall trigger in C++
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseReplyAction::set_CppReplyToCall(const char * v) {
    return set_it_(_cpp_trigger, v, setCppTriggerCmd);
}
#endif

#ifdef WITHJAVA
// return the replyToCall trigger in Java
const QByteArray & UmlBaseReplyAction::javaReplyToCall() {
    read_if_needed_();
    return _java_trigger;
}

// set the replyToCall trigger in Java
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseReplyAction::set_JavaReplyToCall(const char * v) {
    return set_it_(_java_trigger, v, setJavaTriggerCmd);
}
#endif

// to unload the object to free memory, it will be reloaded automatically
// if needed. Recursively done for the sub items if 'rec' is TRUE.
//
// if 'del' is true the sub items are deleted in C++, and removed from the
// internal dictionnary in C++ and Java (to allow it to be garbaged),
// you will have to call Children() to re-access to them
void UmlBaseReplyAction::unload(bool rec, bool del) {
    _trigger = 0;
#ifdef WITHCPP
    _cpp_trigger = 0;
#endif
#ifdef WITHJAVA
    _java_trigger = 0;
#endif
    UmlBaseActivityAction::unload(rec, del);
}

void UmlBaseReplyAction::read_uml_() {
    UmlBaseActivityAction::read_uml_();
    _trigger = UmlCom::read_string();
}

#ifdef WITHCPP
void UmlBaseReplyAction::read_cpp_() {
    UmlBaseActivityAction::read_cpp_();
    _cpp_trigger = UmlCom::read_string();
}
#endif

#ifdef WITHJAVA
void UmlBaseReplyAction::read_java_() {
    UmlBaseActivityAction::read_java_();
    _java_trigger = UmlCom::read_string();
}
#endif

//  returns a new create object action named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlCreateObjectAction * UmlBaseCreateObjectAction::create(UmlItem * parent, const char * s)
{
    return (UmlCreateObjectAction *) parent->create_(aCreateObjectAction, s);
}

// returns the kind of the item
anItemKind UmlBaseCreateObjectAction::kind() {
    return aCreateObjectAction;
}

// return the classifier
const QByteArray & UmlBaseCreateObjectAction::classifier() {
    read_if_needed_();
    return _classifier;
}

// set the classifier
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseCreateObjectAction::set_Classifier(const char * v) {
    return set_it_(_classifier, v, setDefCmd);
}

// to unload the object to free memory, it will be reloaded automatically
// if needed. Recursively done for the sub items if 'rec' is TRUE.
//
// if 'del' is true the sub items are deleted in C++, and removed from the
// internal dictionnary in C++ and Java (to allow it to be garbaged),
// you will have to call Children() to re-access to them
void UmlBaseCreateObjectAction::unload(bool rec, bool del) {
    _classifier = 0;
    UmlBaseActivityAction::unload(rec, del);
}

void UmlBaseCreateObjectAction::read_uml_() {
    UmlBaseActivityAction::read_uml_();
    _classifier = UmlCom::read_string();
}

//  returns a new destroy object action named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlDestroyObjectAction * UmlBaseDestroyObjectAction::create(UmlItem * parent, const char * s)
{
    return (UmlDestroyObjectAction *) parent->create_(aDestroyObjectAction, s);
}

// returns the kind of the item
anItemKind UmlBaseDestroyObjectAction::kind() {
    return aDestroyObjectAction;
}

// return the  return the isDestroyLinks attribute.
bool UmlBaseDestroyObjectAction::isDestroyLinks() {
    read_if_needed_();
    return _links;
}

// set the isDestroyLinks attribute
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseDestroyObjectAction::set_isDestroyLinks(bool v) {
    return set_it_(_links, v, setTypeCmd);
}

// return the  return the isDestroyOwnedObjects attribute
bool UmlBaseDestroyObjectAction::isDestroyOwnedObjects() {
    read_if_needed_();
    return _owned_objects;
}

// set the isDestroyOwnedObjects attribute
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseDestroyObjectAction::set_isDestroyOwnedObjects(bool v) {
    return set_it_(_owned_objects, v, setFlagCmd);
}

void UmlBaseDestroyObjectAction::read_uml_() {
    UmlBaseActivityAction::read_uml_();
    _links = UmlCom::read_bool();
    _owned_objects = UmlCom::read_bool();
}

//  returns a new test identity action named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlTestIdentityAction * UmlBaseTestIdentityAction::create(UmlItem * parent, const char * s)
{
    return (UmlTestIdentityAction *) parent->create_(aTestIdentityAction, s);
}

// returns the kind of the item
anItemKind UmlBaseTestIdentityAction::kind() {
    return aTestIdentityAction;
}

//  returns a new raise exception action named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlRaiseExceptionAction * UmlBaseRaiseExceptionAction::create(UmlItem * parent, const char * s)
{
    return (UmlRaiseExceptionAction *) parent->create_(aRaiseExceptionAction, s);
}

// returns the kind of the item
anItemKind UmlBaseRaiseExceptionAction::kind() {
    return aRaiseExceptionAction;
}

//  returns a new reduce action named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlReduceAction * UmlBaseReduceAction::create(UmlItem * parent, const char * s)
{
    return (UmlReduceAction *) parent->create_(aReduceAction, s);
}

// returns the kind of the item
anItemKind UmlBaseReduceAction::kind() {
    return aReduceAction;
}

// return the  return the isOrdered attribute
bool UmlBaseReduceAction::isOrdered() {
    read_if_needed_();
    return _ordered;
}

// set the isOrdered attribute
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseReduceAction::set_isOrdered(bool v) {
    return set_it_(_ordered, v, setFlagCmd);
}

// return the reducer, may be an activity or a state machine
UmlItem * UmlBaseReduceAction::reducer() {
    read_if_needed_();
    return _reducer;
}

// set the reducer
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseReduceAction::set_Reducer(UmlItem * v) {
    UmlCom::send_cmd(_identifier, setDefCmd, (v == 0) ? (void *) v : ((UmlBaseItem *) v)->_identifier);
    if (UmlCom::read_bool()) {
      _reducer = v;
      return TRUE;
    }
    else
      return FALSE;
}

void UmlBaseReduceAction::read_uml_() {
    UmlBaseActivityAction::read_uml_();
    _ordered = UmlCom::read_bool();
    _reducer = UmlBaseItem::read_();
}

//  returns a new start object behavior action named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlStartObjectBehaviorAction * UmlBaseStartObjectBehaviorAction::create(UmlItem * parent, const char * s)
{
    return (UmlStartObjectBehaviorAction *) parent->create_(aStartObjectBehaviorAction, s);
}

// returns the kind of the item
anItemKind UmlBaseStartObjectBehaviorAction::kind() {
    return aStartObjectBehaviorAction;
}

// return the property 'synchronous'
bool UmlBaseStartObjectBehaviorAction::isSynchronous() {
    read_if_needed_();
    return _synchronous;
}

// set the property 'synchronous'
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseStartObjectBehaviorAction::set_isSynchronous(bool v) {
    return set_it_(_synchronous, v, setFlagCmd);
}

void UmlBaseStartObjectBehaviorAction::read_uml_() {
    UmlBaseActivityAction::read_uml_();
    _synchronous = UmlCom::read_bool();
}

//  returns a new read self action named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlReadSelfAction * UmlBaseReadSelfAction::create(UmlItem * parent, const char * s)
{
    return (UmlReadSelfAction *) parent->create_(aReadSelfAction, s);
}

// returns the kind of the item
anItemKind UmlBaseReadSelfAction::kind() {
    return aReadSelfAction;
}

//  returns a new read extent action named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlReadExtentAction * UmlBaseReadExtentAction::create(UmlItem * parent, const char * s)
{
    return (UmlReadExtentAction *) parent->create_(aReadExtentAction, s);
}

// returns the kind of the item
anItemKind UmlBaseReadExtentAction::kind() {
    return aReadExtentAction;
}

// return the classifier
const QByteArray & UmlBaseReadExtentAction::classifier() {
    read_if_needed_();
    return _classifier;
}

// set the classifier
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseReadExtentAction::set_Classifier(const QByteArray v) {
    return set_it_(_classifier, v, setUmlActivityCmd);
}

void UmlBaseReadExtentAction::read_uml_() {
    UmlBaseActivityAction::read_uml_();
    _classifier = UmlCom::read_string();
}

//  returns a new reclassify object action named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlReclassifyObjectAction * UmlBaseReclassifyObjectAction::create(UmlItem * parent, const char * s)
{
    return (UmlReclassifyObjectAction *) parent->create_(aReclassifyObjectAction, s);
}

// returns the kind of the item
anItemKind UmlBaseReclassifyObjectAction::kind() {
    return aReclassifyObjectAction;
}

// return the property 'replaceAll'
bool UmlBaseReclassifyObjectAction::isReplaceAll() {
    read_if_needed_();
    return _replace_all;
}

// set the property 'replaceAll'
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseReclassifyObjectAction::set_isReplaceAll(bool v) {
    return set_it_(_replace_all, v, setFlagCmd);
}

// return the old classifiers
const QByteArray & UmlBaseReclassifyObjectAction::oldClassifier() {
    read_if_needed_();
    return _old_classifier;
}

// set the old classifier
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseReclassifyObjectAction::set_OldClassifier(const QByteArray v) {
    return set_it_(_old_classifier, v, setUmlActivityCmd);
}

// return the new classifiers
const QByteArray & UmlBaseReclassifyObjectAction::newClassifier() {
    read_if_needed_();
    return _new_classifier;
}

// set the new classifier
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseReclassifyObjectAction::set_NewClassifier(const QByteArray v) {
    return set_it_(_new_classifier, v, setUmlGuardCmd);
}

void UmlBaseReclassifyObjectAction::read_uml_() {
    UmlBaseActivityAction::read_uml_();
    _replace_all = UmlCom::read_bool();
    _old_classifier = UmlCom::read_string();
    _new_classifier = UmlCom::read_string();
}

//  returns a new read is classified object action named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlReadIsClassifiedObjectAction * UmlBaseReadIsClassifiedObjectAction::create(UmlItem * parent, const char * s)
{
    return (UmlReadIsClassifiedObjectAction *) parent->create_(aReadIsClassifiedObjectAction, s);
}

// returns the kind of the item
anItemKind UmlBaseReadIsClassifiedObjectAction::kind() {
    return aReadIsClassifiedObjectAction;
}

// return the property 'direct'
bool UmlBaseReadIsClassifiedObjectAction::isDirect() {
    read_if_needed_();
    return _direct;
}

// set the property 'direct'
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseReadIsClassifiedObjectAction::set_isDirect(bool v) {
    return set_it_(_direct, v, setFlagCmd);
}

// return the classifier
const QByteArray & UmlBaseReadIsClassifiedObjectAction::classifier() {
    read_if_needed_();
    return _classifier;
}

// set the classifier
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseReadIsClassifiedObjectAction::set_Classifier(const QByteArray v) {
    return set_it_(_classifier, v, setUmlActivityCmd);
}

void UmlBaseReadIsClassifiedObjectAction::read_uml_() {
    UmlBaseActivityAction::read_uml_();
    _direct = UmlCom::read_bool();
    _classifier = UmlCom::read_string();
}

//  returns a new start classifier behavior action named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlStartClassifierBehaviorAction * UmlBaseStartClassifierBehaviorAction::create(UmlItem * parent, const char * s)
{
    return (UmlStartClassifierBehaviorAction *) parent->create_(aStartClassifierBehaviorAction, s);
}

// returns the kind of the item
anItemKind UmlBaseStartClassifierBehaviorAction::kind() {
    return aStartClassifierBehaviorAction;
}

//  returns a new read structural feature action named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlReadStructuralFeatureAction * UmlBaseReadStructuralFeatureAction::create(UmlStructuralFeatureAction * parent, const char * s)
{
    return (UmlReadStructuralFeatureAction *) parent->create_(aReadStructuralFeatureAction, s);
}

// returns the kind of the item
anItemKind UmlBaseReadStructuralFeatureAction::kind() {
    return aReadStructuralFeatureAction;
}

//  returns a new clear structural feature action named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlClearStructuralFeatureAction * UmlBaseClearStructuralFeatureAction::create(UmlStructuralFeatureAction * parent, const char * s)
{
    return (UmlClearStructuralFeatureAction *) parent->create_(aClearStructuralFeatureAction, s);
}

// returns the kind of the item
anItemKind UmlBaseClearStructuralFeatureAction::kind() {
    return aClearStructuralFeatureAction;
}

//  returns a new add structural feature action named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlAddStructuralFeatureValueAction * UmlBaseAddStructuralFeatureValueAction::create(UmlWriteStructuralFeatureAction * parent, const char * s)
{
    return (UmlAddStructuralFeatureValueAction *) parent->create_(anAddStructuralFeatureValueAction, s);
}

// returns the kind of the item
anItemKind UmlBaseAddStructuralFeatureValueAction::kind() {
    return anAddStructuralFeatureValueAction;
}

// return the property 'replaceAll'
bool UmlBaseAddStructuralFeatureValueAction::isReplaceAll() {
    read_if_needed_();
    return _replace_all;
}

// set the property 'replaceAll'
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseAddStructuralFeatureValueAction::set_isReplaceAll(bool v) {
    return set_it_(_replace_all, v, setFlagCmd);
}

void UmlBaseAddStructuralFeatureValueAction::read_uml_() {
    UmlBaseWriteStructuralFeatureAction::read_uml_();
    _replace_all = UmlCom::read_bool();
}

//  returns a new remove structural feature action named 's' created under 'parent'
//
// In case it cannot be created (the name is already used or
// invalid, 'parent' cannot contain it etc ...) return 0 in C++
// and produce a RuntimeException in Java
UmlRemoveStructuralFeatureValueAction * UmlBaseRemoveStructuralFeatureValueAction::create(UmlWriteStructuralFeatureAction * parent, const char * s)
{
    return (UmlRemoveStructuralFeatureValueAction *) parent->create_(aRemoveStructuralFeatureValueAction, s);
}

// returns the kind of the item
anItemKind UmlBaseRemoveStructuralFeatureValueAction::kind() {
    return aRemoveStructuralFeatureValueAction;
}

// return the property 'removeDuplicates'
bool UmlBaseRemoveStructuralFeatureValueAction::isRemoveDuplicates() {
    read_if_needed_();
    return _remove_duplicates;
}

// set the property 'removeDuplicates'
//
// On error return FALSE in C++, produce a RuntimeException in Java
bool UmlBaseRemoveStructuralFeatureValueAction::set_isRemoveDuplicates(bool v) {
    return set_it_(_remove_duplicates, v, setFlagCmd);
}

void UmlBaseRemoveStructuralFeatureValueAction::read_uml_() {
    UmlBaseWriteStructuralFeatureAction::read_uml_();
    _remove_duplicates = UmlCom::read_bool();
}


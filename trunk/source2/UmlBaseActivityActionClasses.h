#ifndef _UMLBASEACTIVITYACTIONCLASSES_H
#define _UMLBASEACTIVITYACTIONCLASSES_H


#include "UmlActivityAction.h"
#include "anItemKind.h"
#include <qbytearray.h>
#include "UmlOnSignalAction.h"
#include "UmlAccessVariableValueAction.h"
#include "UmlStructuralFeatureAction.h"
#include "UmlWriteStructuralFeatureAction.h"

class UmlSendObjectAction;
class UmlItem;
class UmlUnmarshallAction;
class UmlSendSignalAction;
class UmlBroadcastSignalAction;
class UmlValueSpecificationAction;
class UmlOpaqueAction;
class UmlAcceptEventAction;
class UmlCallOperationAction;
class UmlOperation;
class UmlCallBehaviorAction;
class UmlClearVariableValueAction;
class UmlReadVariableValueAction;
class UmlAddVariableValueAction;
class UmlRemoveVariableValueAction;
class UmlAcceptCallAction;
class UmlReplyAction;
class UmlCreateObjectAction;
class UmlDestroyObjectAction;
class UmlTestIdentityAction;
class UmlRaiseExceptionAction;
class UmlReduceAction;
class UmlStartObjectBehaviorAction;
class UmlReadSelfAction;
class UmlReadExtentAction;
class UmlReclassifyObjectAction;
class UmlReadIsClassifiedObjectAction;
class UmlStartClassifierBehaviorAction;
class UmlReadStructuralFeatureAction;
class UmlClearStructuralFeatureAction;
class UmlAddStructuralFeatureValueAction;
class UmlRemoveStructuralFeatureValueAction;

class UmlBaseSendObjectAction : public UmlActivityAction {
  public:
    //  returns a new send object action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlSendObjectAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseSendObjectAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }

};

class UmlBaseUnmarshallAction : public UmlActivityAction {
  public:
    //  returns a new unmarshall action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlUnmarshallAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseUnmarshallAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the classifier
    const QByteArray & classifier();

    // set the classifier
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Classifier(const QByteArray v);


  private:
    QByteArray _classifier;


  protected:
    void read_uml_();

};

class UmlBaseSendSignalAction : public UmlOnSignalAction {
  public:
    //  returns a new send signal action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlSendSignalAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseSendSignalAction(void * id, const QByteArray & s) : UmlOnSignalAction(id, s) {
    }

};

class UmlBaseBroadcastSignalAction : public UmlOnSignalAction {
  public:
    //  returns a new broadcast signal action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlBroadcastSignalAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseBroadcastSignalAction(void * id, const QByteArray & s) : UmlOnSignalAction(id, s) {
    }

};

class UmlBaseValueSpecificationAction : public UmlActivityAction {
  public:
    //  returns a new value specification action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlValueSpecificationAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseValueSpecificationAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the value
    const QByteArray & value();

    // set the value
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Value(const char * v);

#ifdef WITHCPP
    // return the value in C++
    const QByteArray & cppValue();

    // set the value in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppValue(const char * v);
#endif

#ifdef WITHJAVA
    // return the value in Java
    const QByteArray & javaValue();

    // set the value in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaValue(const char * v);
#endif

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE. 
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    QByteArray _value;

#ifdef WITHCPP
    QByteArray _cpp_value;
#endif

#ifdef WITHJAVA
    QByteArray _java_value;
#endif


  protected:
    virtual void read_uml_();

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    virtual void read_java_();
#endif

};

class UmlBaseOpaqueAction : public UmlActivityAction {
  public:
    //  returns a new opaque action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlOpaqueAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseOpaqueAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the behavior
    const QByteArray & behavior();

    // set the behavior
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Behavior(const char * v);

#ifdef WITHCPP
    // return the behavior in C++
    const QByteArray & cppBehavior();

    // set the behavior in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppBehavior(const char * v);
#endif

#ifdef WITHJAVA
    // return the behavior in Java
    const QByteArray & javaBehavior();

    // set the behavior in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaBehavior(const char * v);
#endif

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE. 
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    QByteArray _behavior;

#ifdef WITHCPP
    QByteArray _cpp_behavior;
#endif

#ifdef WITHJAVA
    QByteArray _java_behavior;
#endif


  protected:
    virtual void read_uml_();

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    virtual void read_java_();
#endif

};

class UmlBaseAcceptEventAction : public UmlActivityAction {
  public:
    //  returns a new accept event action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlAcceptEventAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseAcceptEventAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the isUnmarshall attribute, if TRUE there are multiple output pins for attributes or the event.
    bool isUnmarshall();

    // set the isUnmarshall attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isUnmarshall(bool v);

    // return the isTimeEvent attribute, if TRUE the event is a time event
    bool isTimeEvent();

    // set the isTimeEvent attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isTimeEvent(bool v);

    // return the trigger
    const QByteArray & trigger();

    // set the trigger
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Trigger(const char * v);

#ifdef WITHCPP
    // return the trigger in C++
    const QByteArray & cppTrigger();

    // set the trigger in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppTrigger(const char * v);
#endif

#ifdef WITHJAVA
    // return the trigger in Java
    const QByteArray & javaTrigger();

    // set the trigger in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaTrigger(const char * v);
#endif

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE. 
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    bool _unmarshall;

    bool _timeevent;

    QByteArray _trigger;

#ifdef WITHCPP
    QByteArray _cpp_trigger;
#endif

#ifdef WITHJAVA
    QByteArray _java_trigger;
#endif


  protected:
    virtual void read_uml_();

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    virtual void read_java_();
#endif

};

class UmlBaseCallOperationAction : public UmlActivityAction {
  public:
    //  returns a new call operation action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlCallOperationAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseCallOperationAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the isSynchronous attribute, if TRUE the caller waits for the completion of the invoked behavior
    bool isSynchronous();

    // set the isSynchronous attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isSynchronous(bool v);

    // return the operation
    UmlOperation * operation();

    // set the operation
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Operation(UmlOperation * v);


  private:
    bool _synchronous;

    UmlOperation * _operation;


  protected:
    virtual void read_uml_();

};

class UmlBaseCallBehaviorAction : public UmlActivityAction {
  public:
    //  returns a new call behavior action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlCallBehaviorAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseCallBehaviorAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the isSynchronous attribute, if TRUE the caller waits for the completion of the invoked behavior
    bool isSynchronous();

    // set the isSynchronous attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isSynchronous(bool v);

    // return the behavior, may be an activity or a state machine
    UmlItem * behavior();

    // set the behavior
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Behavior(UmlItem * v);


  private:
    bool _synchronous;

    UmlItem * _behavior;


  protected:
    virtual void read_uml_();

};

class UmlBaseClearVariableValueAction : public UmlAccessVariableValueAction {
  public:
    //  returns a new clear variable value action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlClearVariableValueAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseClearVariableValueAction(void * id, const QByteArray & s) : UmlAccessVariableValueAction(id, s) {
    }

};

class UmlBaseReadVariableValueAction : public UmlAccessVariableValueAction {
  public:
    //  returns a new read variable value action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlReadVariableValueAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseReadVariableValueAction(void * id, const QByteArray & s) : UmlAccessVariableValueAction(id, s) {
    }

};

class UmlBaseAddVariableValueAction : public UmlAccessVariableValueAction {
  public:
    //  returns a new add variable value action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlAddVariableValueAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseAddVariableValueAction(void * id, const QByteArray & s) : UmlAccessVariableValueAction(id, s) {
    }


  public:
    // return the isReplaceAll attribute, if TRUE existing values of the variable must be removed before adding the new value
    bool isReplaceAll();

    // set the isReplaceAll attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isReplaceAll(bool v);


  private:
    bool _replace_all;


  protected:
    virtual void read_uml_();

};

class UmlBaseRemoveVariableValueAction : public UmlAccessVariableValueAction {
  public:
    //  returns a new remove variable value action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlRemoveVariableValueAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseRemoveVariableValueAction(void * id, const QByteArray & s) : UmlAccessVariableValueAction(id, s) {
    }


  public:
    // return the isRemoveDuplicates attribute, if TRUE remove duplicates of the value if non-unique
    bool isRemoveDuplicates();

    // set the isRemoveDuplicates attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isRemoveDuplicates(bool v);


  private:
    bool _remove_duplicates;


  protected:
    virtual void read_uml_();

};

class UmlBaseAcceptCallAction : public UmlActivityAction {
  public:
    //  returns a new accept call action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlAcceptCallAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseAcceptCallAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the property 'unmarshall'
    bool isUnmarshall();

    // set the property 'unmarshall'
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isUnmarshall(bool v);

    // return the trigger
    const QByteArray & trigger();

    // set the trigger
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Trigger(const char * v);

#ifdef WITHCPP
    // return the trigger in C++
    const QByteArray & cppTrigger();

    // set the trigger in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppTrigger(const char * v);
#endif

#ifdef WITHJAVA
    // return the trigger in Java
    const QByteArray & javaTrigger();

    // set the trigger in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaTrigger(const char * v);
#endif

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE.
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    bool _unmarshall;

    QByteArray _trigger;

#ifdef WITHCPP
    QByteArray _cpp_trigger;
#endif

#ifdef WITHJAVA
    QByteArray _java_trigger;
#endif


  protected:
    virtual void read_uml_();

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    virtual void read_java_();
#endif

};
class UmlBaseReplyAction : public UmlActivityAction {
  public:
    //  returns a new reply action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlReplyAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseReplyAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the replyToCall trigger
    const QByteArray & replyToCall();

    // set the replyToCall trigger
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_ReplyToCall(const char * v);

#ifdef WITHCPP
    // return the replyToCall trigger in C++
    const QByteArray & cppReplyToCall();

    // set the replyToCall trigger in C++
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_CppReplyToCall(const char * v);
#endif

#ifdef WITHJAVA
    // return the replyToCall trigger in Java
    const QByteArray & javaReplyToCall();

    // set the replyToCall trigger in Java
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_JavaReplyToCall(const char * v);
#endif

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE.
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    QByteArray _trigger;

#ifdef WITHCPP
    QByteArray _cpp_trigger;
#endif

#ifdef WITHJAVA
    QByteArray _java_trigger;
#endif


  protected:
    virtual void read_uml_();

#ifdef WITHCPP
    virtual void read_cpp_();
#endif

#ifdef WITHJAVA
    virtual void read_java_();
#endif

};
class UmlBaseCreateObjectAction : public UmlActivityAction {
  public:
    //  returns a new create object action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlCreateObjectAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseCreateObjectAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the classifier
    const QByteArray & classifier();

    // set the classifier
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Classifier(const char * v);

    // to unload the object to free memory, it will be reloaded automatically
    // if needed. Recursively done for the sub items if 'rec' is TRUE.
    //
    // if 'del' is true the sub items are deleted in C++, and removed from the
    // internal dictionnary in C++ and Java (to allow it to be garbaged),
    // you will have to call Children() to re-access to them
    virtual void unload(bool = FALSE, bool = FALSE);


  private:
    QByteArray _classifier;


  protected:
    virtual void read_uml_();

};
class UmlBaseDestroyObjectAction : public UmlActivityAction {
  public:
    //  returns a new destroy object action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlDestroyObjectAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseDestroyObjectAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the  return the isDestroyLinks attribute.
    bool isDestroyLinks();

    // set the isDestroyLinks attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isDestroyLinks(bool v);

    // return the  return the isDestroyOwnedObjects attribute
    bool isDestroyOwnedObjects();

    // set the isDestroyOwnedObjects attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isDestroyOwnedObjects(bool v);


  private:
    bool _links;

    bool _owned_objects;


  protected:
    virtual void read_uml_();

};
class UmlBaseTestIdentityAction : public UmlActivityAction {
  public:
    //  returns a new test identity action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlTestIdentityAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseTestIdentityAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }

};
class UmlBaseRaiseExceptionAction : public UmlActivityAction {
  public:
    //  returns a new raise exception action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlRaiseExceptionAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseRaiseExceptionAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }

};
class UmlBaseReduceAction : public UmlActivityAction {
  public:
    //  returns a new reduce action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlReduceAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseReduceAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }


  public:
    // return the  return the isOrdered attribute
    bool isOrdered();

    // set the isOrdered attribute
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isOrdered(bool v);

    // return the reducer, may be an activity or a state machine
    UmlItem * reducer();

    // set the reducer
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Reducer(UmlItem * v);


  private:
    bool _ordered;

    UmlItem * _reducer;


  protected:
    virtual void read_uml_();

};
class UmlBaseStartObjectBehaviorAction : public UmlActivityAction {
  public:
    //  returns a new start object behavior action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlStartObjectBehaviorAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();

    // return the property 'synchronous'
    bool isSynchronous();

    // set the property 'synchronous'
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isSynchronous(bool v);


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseStartObjectBehaviorAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }

    void read_uml_();


  private:
    bool _synchronous;

};
class UmlBaseReadSelfAction : public UmlActivityAction {
  public:
    //  returns a new read self action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlReadSelfAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseReadSelfAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }

};
class UmlBaseReadExtentAction : public UmlActivityAction {
  public:
    //  returns a new read extent action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlReadExtentAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();

    // return the classifier
    const QByteArray & classifier();

    // set the classifier
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Classifier(const QByteArray v);


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseReadExtentAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }

    void read_uml_();


  private:
    QByteArray _classifier;

};
class UmlBaseReclassifyObjectAction : public UmlActivityAction {
  public:
    //  returns a new reclassify object action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlReclassifyObjectAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();

    // return the property 'replaceAll'
    bool isReplaceAll();

    // set the property 'replaceAll'
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isReplaceAll(bool v);

    // return the old classifiers
    const QByteArray & oldClassifier();

    // set the old classifier
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_OldClassifier(const QByteArray v);

    // return the new classifiers
    const QByteArray & newClassifier();

    // set the new classifier
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_NewClassifier(const QByteArray v);


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseReclassifyObjectAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }

    void read_uml_();


  private:
    bool _replace_all;

    QByteArray _old_classifier;

    QByteArray _new_classifier;

};
class UmlBaseReadIsClassifiedObjectAction : public UmlActivityAction {
  public:
    //  returns a new read is classified object action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlReadIsClassifiedObjectAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();

    // return the property 'direct'
    bool isDirect();

    // set the property 'direct'
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isDirect(bool v);

    // return the classifier
    const QByteArray & classifier();

    // set the classifier
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_Classifier(const QByteArray v);


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseReadIsClassifiedObjectAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }

    void read_uml_();


  private:
    bool _direct;

    QByteArray _classifier;

};
class UmlBaseStartClassifierBehaviorAction : public UmlActivityAction {
  public:
    //  returns a new start classifier behavior action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlStartClassifierBehaviorAction * create(UmlItem * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseStartClassifierBehaviorAction(void * id, const QByteArray & s) : UmlActivityAction(id, s) {
    }

};
class UmlBaseReadStructuralFeatureAction : public UmlStructuralFeatureAction {
  public:
    //  returns a new read structural feature action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlReadStructuralFeatureAction * create(UmlStructuralFeatureAction * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseReadStructuralFeatureAction(void * id, const QByteArray & s) : UmlStructuralFeatureAction(id, s) {
    }

};
class UmlBaseClearStructuralFeatureAction : public UmlStructuralFeatureAction {
  public:
    //  returns a new clear structural feature action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlClearStructuralFeatureAction * create(UmlStructuralFeatureAction * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseClearStructuralFeatureAction(void * id, const QByteArray & s) : UmlStructuralFeatureAction(id, s) {
    }

};
class UmlBaseAddStructuralFeatureValueAction : public UmlWriteStructuralFeatureAction {
  public:
    //  returns a new add structural feature action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlAddStructuralFeatureValueAction * create(UmlWriteStructuralFeatureAction * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();

    // return the property 'replaceAll'
    bool isReplaceAll();

    // set the property 'replaceAll'
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isReplaceAll(bool v);


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseAddStructuralFeatureValueAction(void * id, const QByteArray & s) : UmlWriteStructuralFeatureAction(id, s) {
    }

    void read_uml_();


  private:
    bool _replace_all;

};
class UmlBaseRemoveStructuralFeatureValueAction : public UmlWriteStructuralFeatureAction {
  public:
    //  returns a new remove structural feature action named 's' created under 'parent'
    //
    // In case it cannot be created (the name is already used or
    // invalid, 'parent' cannot contain it etc ...) return 0 in C++
    // and produce a RuntimeException in Java
    static UmlRemoveStructuralFeatureValueAction * create(UmlWriteStructuralFeatureAction * parent, const char * s);

    // returns the kind of the item
    virtual anItemKind kind();

    // return the property 'removeDuplicates'
    bool isRemoveDuplicates();

    // set the property 'removeDuplicates'
    //
    // On error return FALSE in C++, produce a RuntimeException in Java
    bool set_isRemoveDuplicates(bool v);


  protected:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBaseRemoveStructuralFeatureValueAction(void * id, const QByteArray & s) : UmlWriteStructuralFeatureAction(id, s) {
    }

    void read_uml_();


  private:
    bool _remove_duplicates;

};
#endif

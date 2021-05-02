#ifndef _UMLACTIVITYACTIONCLASSES_H
#define _UMLACTIVITYACTIONCLASSES_H


#include "UmlBaseActivityActionClasses.h"
#include <qbytearray.h>

class UmlSendObjectAction : public UmlBaseSendObjectAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlSendObjectAction(void * id, const QByteArray & s) : UmlBaseSendObjectAction(id, s) {
    }

};

class UmlUnmarshallAction : public UmlBaseUnmarshallAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlUnmarshallAction(void * id, const QByteArray & s) : UmlBaseUnmarshallAction(id, s) {
    }

};

class UmlSendSignalAction : public UmlBaseSendSignalAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlSendSignalAction(void * id, const QByteArray & s) : UmlBaseSendSignalAction(id, s) {
    }

};

class UmlBroadcastSignalAction : public UmlBaseBroadcastSignalAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlBroadcastSignalAction(void * id, const QByteArray & s) : UmlBaseBroadcastSignalAction(id, s) {
    }

};

class UmlValueSpecificationAction : public UmlBaseValueSpecificationAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlValueSpecificationAction(void * id, const QByteArray & s) : UmlBaseValueSpecificationAction(id, s) {
    }

};

class UmlOpaqueAction : public UmlBaseOpaqueAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlOpaqueAction(void * id, const QByteArray & s) : UmlBaseOpaqueAction(id, s) {
    }

};

class UmlAcceptEventAction : public UmlBaseAcceptEventAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlAcceptEventAction(void * id, const QByteArray & s) : UmlBaseAcceptEventAction(id, s) {
    }

};

class UmlCallOperationAction : public UmlBaseCallOperationAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlCallOperationAction(void * id, const QByteArray & s) : UmlBaseCallOperationAction(id, s) {
    }

};

class UmlCallBehaviorAction : public UmlBaseCallBehaviorAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlCallBehaviorAction(void * id, const QByteArray & s) : UmlBaseCallBehaviorAction(id, s) {
    }

};

class UmlClearVariableValueAction : public UmlBaseClearVariableValueAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlClearVariableValueAction(void * id, const QByteArray & s) : UmlBaseClearVariableValueAction(id, s) {
    }

};

class UmlReadVariableValueAction : public UmlBaseReadVariableValueAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlReadVariableValueAction(void * id, const QByteArray & s) : UmlBaseReadVariableValueAction(id, s) {
    }

};

class UmlAddVariableValueAction : public UmlBaseAddVariableValueAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlAddVariableValueAction(void * id, const QByteArray & s) : UmlBaseAddVariableValueAction(id, s) {
    }

};

class UmlRemoveVariableValueAction : public UmlBaseRemoveVariableValueAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlRemoveVariableValueAction(void * id, const QByteArray & s) : UmlBaseRemoveVariableValueAction(id, s) {
    }

};

class UmlAcceptCallAction : public UmlBaseAcceptCallAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlAcceptCallAction(void * id, const QByteArray & s) : UmlBaseAcceptCallAction(id, s) {
    }

};
class UmlReplyAction : public UmlBaseReplyAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlReplyAction(void * id, const QByteArray & s) : UmlBaseReplyAction(id, s) {
    }

};
class UmlCreateObjectAction : public UmlBaseCreateObjectAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlCreateObjectAction(void * id, const QByteArray & s) : UmlBaseCreateObjectAction(id, s) {
    }

};
class UmlDestroyObjectAction : public UmlBaseDestroyObjectAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlDestroyObjectAction(void * id, const QByteArray & s) : UmlBaseDestroyObjectAction(id, s) {
    }

};
class UmlTestIdentityAction : public UmlBaseTestIdentityAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlTestIdentityAction(void * id, const QByteArray & s) : UmlBaseTestIdentityAction(id, s) {
    }

};
class UmlRaiseExceptionAction : public UmlBaseRaiseExceptionAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlRaiseExceptionAction(void * id, const QByteArray & s) : UmlBaseRaiseExceptionAction(id, s) {
    }

};
class UmlReduceAction : public UmlBaseReduceAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlReduceAction(void * id, const QByteArray & s) : UmlBaseReduceAction(id, s) {
    }

};
class UmlStartObjectBehaviorAction : public UmlBaseStartObjectBehaviorAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlStartObjectBehaviorAction(void * id, const QByteArray & s) : UmlBaseStartObjectBehaviorAction(id, s) {
    }

};
class UmlReadSelfAction : public UmlBaseReadSelfAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlReadSelfAction(void * id, const QByteArray & s) : UmlBaseReadSelfAction(id, s) {
    }

};
class UmlReadExtentAction : public UmlBaseReadExtentAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlReadExtentAction(void * id, const QByteArray & s) : UmlBaseReadExtentAction(id, s) {
    }

};
class UmlReclassifyObjectAction : public UmlBaseReclassifyObjectAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlReclassifyObjectAction(void * id, const QByteArray & s) : UmlBaseReclassifyObjectAction(id, s) {
    }

};
class UmlReadIsClassifiedObjectAction : public UmlBaseReadIsClassifiedObjectAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlReadIsClassifiedObjectAction(void * id, const QByteArray & s) : UmlBaseReadIsClassifiedObjectAction(id, s) {
    }

};
class UmlStartClassifierBehaviorAction : public UmlBaseStartClassifierBehaviorAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlStartClassifierBehaviorAction(void * id, const QByteArray & s) : UmlBaseStartClassifierBehaviorAction(id, s) {
    }

};
class UmlReadStructuralFeatureAction : public UmlBaseReadStructuralFeatureAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlReadStructuralFeatureAction(void * id, const QByteArray & s) : UmlBaseReadStructuralFeatureAction(id, s) {
    }

};
class UmlClearStructuralFeatureAction : public UmlBaseClearStructuralFeatureAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlClearStructuralFeatureAction(void * id, const QByteArray & s) : UmlBaseClearStructuralFeatureAction(id, s) {
    }

};
class UmlAddStructuralFeatureValueAction : public UmlBaseAddStructuralFeatureValueAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlAddStructuralFeatureValueAction(void * id, const QByteArray & s) : UmlBaseAddStructuralFeatureValueAction(id, s) {
    }

};
class UmlRemoveStructuralFeatureValueAction : public UmlBaseRemoveStructuralFeatureValueAction {
  public:
    //  the constructor, do not call it yourself !!!!!!!!!!
     UmlRemoveStructuralFeatureValueAction(void * id, const QByteArray & s) : UmlBaseRemoveStructuralFeatureValueAction(id, s) {
    }

};
#endif

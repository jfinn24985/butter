#ifndef _UMLACTIVITYCONTROLNODECLASSES_H
#define _UMLACTIVITYCONTROLNODECLASSES_H


#include "bouml/UmlBaseActivityControlNodeClasses.h"
#include <qbytearray.h>

class UmlInitialActivityNode : public UmlBaseInitialActivityNode {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlInitialActivityNode(void * id, const QByteArray & s) : UmlBaseInitialActivityNode(id, s) {
    }

};

class UmlFlowFinalActivityNode : public UmlBaseFlowFinalActivityNode {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlFlowFinalActivityNode(void * id, const QByteArray & s) : UmlBaseFlowFinalActivityNode(id, s) {
    }

};

class UmlActivityFinalActivityNode : public UmlBaseActivityFinalActivityNode {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlActivityFinalActivityNode(void * id, const QByteArray & s) : UmlBaseActivityFinalActivityNode(id, s) {
    }

};

class UmlDecisionActivityNode : public UmlBaseDecisionActivityNode {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlDecisionActivityNode(void * id, const QByteArray & s) : UmlBaseDecisionActivityNode(id, s) {
    }

};

class UmlMergeActivityNode : public UmlBaseMergeActivityNode {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlMergeActivityNode(void * id, const QByteArray & s) : UmlBaseMergeActivityNode(id, s) {
    }

};

class UmlForkActivityNode : public UmlBaseForkActivityNode {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlForkActivityNode(void * id, const QByteArray & s) : UmlBaseForkActivityNode(id, s) {
    }

};

class UmlJoinActivityNode : public UmlBaseJoinActivityNode {
  public:
    /**
     *   the constructor, do not call it yourself !!!!!!!!!!
     */
     UmlJoinActivityNode(void * id, const QByteArray & s) : UmlBaseJoinActivityNode(id, s) {
    }

};

#endif

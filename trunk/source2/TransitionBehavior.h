#ifndef _TRANSITIONBEHAVIOR_H
#define _TRANSITIONBEHAVIOR_H


#include <qbytearray.h>

class TransitionBehavior {
  public:
    QByteArray trigger;

    QByteArray guard;

    QByteArray activity;

    void read();

    void unload();

};

#endif

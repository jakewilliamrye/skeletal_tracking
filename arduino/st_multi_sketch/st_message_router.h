#ifndef ST_MESSAGE_ROUTER_H_
#define ST_MESSAGE_ROUTER_H_

#include "st_message.h"

class StMessageRoute {
  public:
    StMessageRoute(int tag, void (*f)(StMessage m)) {
      this->tag = tag;
      this->f = f;
    };
    int tag;
    void (*f)(StMessage m);
};
//StMessageRouter is responsible for routing StMessages received from the StMessageReceiver to specific callback functions, which are assigned using the attachCallback function.
class StMessageRouter {
  private:
   StMessageRoute routes[25];
  public:
    void attachCallback(int tag, void (*f)(StMessage m));
    void attachRoute(StMessageRoute r);
    void handleNewMessage(StMessage m);
};

#endif

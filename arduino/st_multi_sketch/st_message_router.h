#ifndef ST_MESSAGE_ROUTER_H_
#define ST_MESSAGE_ROUTER_H_

//StMessageRouter is responsible for routing StMessages received from the StMessageReceiver to specific callback functions, which are assigned using the attachCallback function.
class StMessageRouter {
  public:
    void attachCallback(int tag, int callback_function);
};

#endif

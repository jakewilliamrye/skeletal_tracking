#ifndef ST_MESSAGE_SENDER_H_
#define ST_MESSAGE_SENDER_H_

//StMessageSender takes StMessages, and applies a sending protocol and sends them over serial
class StMessageSender {
  public:
    void send(StMessage m);
};

#endif

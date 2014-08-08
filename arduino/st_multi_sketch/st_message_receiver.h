#ifndef ST_MESSAGE_RECEIVER_H_
#define ST_MESSAGE_SENDER_H_

#include "st_message.h"
#include "st_message_router.h"

//StMessageReceiver handles the state machine behind received st messages that are transmitted using a specific StMessageTransferProtocol
//It is also responsable for generating a single callback function when a message is received.
class StMessageReceiver {
  private:
    int state;
    //Holds the message beign received, after the message is parsed, this is sent to the callback function.
    StMessage m;
    //Holds the router being used for the message receiver, the router determines what callbacks are generated when a message is received.
    StMessageRouter r;
  public:
    void parse(uint8_t c);
};
#endif

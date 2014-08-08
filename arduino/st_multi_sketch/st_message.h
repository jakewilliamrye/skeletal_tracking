#ifndef ST_MESSAGE_H_
#define ST_MESSAGE_H_

#include "st_message_header.h"

//StMessage is the base class that stores a serialized message, which can be interpreted by the
//St messaging protocol
class StMessage {
  private:
    uint8_t data[256];
    uint8_t size;
    //Returns 1 if the message was successfully packed
    template<class T>
    int pack(T& object, StMessageHeader h); 
    
    template<class T>
    int pack(T& object, int tag);
};

#endif

#ifndef ST_MESSAGE_H_
#define ST_MESSAGE_H_

#include "st_message_header.h"

//StMessage is the base class that stores a serialized message, which can be interpreted by the
//St messaging protocol
class StMessage {
  public:
    uint8_t data[256];
    uint16_t size;
    StMessage(){};
    template<class H, class T>
    void pack(H h, T& object) {
      //Pack the header
      for(int i=0;i < sizeof(h); i++)
        this->data[i] = *((uint8_t*)&h+i);
      //Pack the payload
      for(int i=0;i < sizeof(object);i++)
        this->data[i+sizeof(h)] = *((uint8_t*)&object+i);
        
      this->size = sizeof(h)+sizeof(object);
    };
    template<class T>
    void pack(int tag, T& object) {
      StMessageHeader h;
      h.tag = tag;
      h.payload_size = sizeof(object);
      this->pack(h,object);
    };
};

#endif

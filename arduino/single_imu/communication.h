#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial ser(10,11);

class Header {
  public:
    Header() {};
    Header(int tag, int payload_size) {
      this->tag = tag;
      this->payload_size = payload_size;
    }
    template<class T>
    Header(int tag, T& object) {
      this->tag = tag;
      this->payload_size = sizeof(object);
    }
    uint16_t tag;
    uint16_t payload_size;
};

class Message {
  public:
    Message(){};
    template<class T>
    Message(Header h, T& object) {
      this->pack(h, object);
    }
    uint8_t data[256];
    uint16_t size;
    template<class H, class T>
    void pack(H h, T& object) {
      //Pack the header
      for(int i=0;i < sizeof(h); i++)
        this->data[i] = *((uint8_t*)&h+i);
      //Pack the payload
      for(int i=0;i < sizeof(object);i++)
        this->data[i+sizeof(h)] = *((uint8_t*)&object+i);
        
      this->size = sizeof(h)+sizeof(object);
    }
    template<class T>
    void pack(int tag, T& object) {
      Header h;
      h.tag = tag;
      h.payload_size = sizeof(object);
      this->pack(h,object);
    }
};


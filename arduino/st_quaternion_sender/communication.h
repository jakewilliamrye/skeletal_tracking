#include <Arduino.h>

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

/*
class Message {
  public:
    uint16_t id;
    uint16_t payload_size;
    uint8_t* payload;
    uint8_t checksum;
    template<class T>
    void pack(uint16_t id, T& object){
      this->id = id;
      this->payload_size = sizeof(object);
      this->payload = (uint8_t*)(void*)&object;
      //Calculate checksum as XOR of all bytes in header
      //and payload
      checksum = 0;
      uint8_t* p = (uint8_t*)(void*)&id;
      for(int i=0;i<sizeof(this->id);i++)
        checksum ^= *(p+i);
        
      p = (uint8_t*)(void*)&payload_size;
      for(int i=0;i<sizeof(this->payload_size);i++)
        checksum ^= *(p+i);
        
      for(int i=0;i<payload_size;i++)
        checksum ^= *(payload+i);
    };
    void send(){
      //Write start byte
      ser.write((uint8_t)'s');
      //Write Header 
      ser.write((uint8_t*)(void*)&this->id,sizeof(this->id));
      //Serial.write((uint8_t*)(void*)this->id, sizeof(this->id));
      ser.write((uint8_t*)(void*)&this->payload_size,sizeof(this->payload_size));
      //Write payload
      ser.write(this->payload,this->payload_size);
      //Write checksum
      ser.write(checksum);
    };
};*/


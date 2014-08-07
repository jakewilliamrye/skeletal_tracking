#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

#include <Arduino.h>

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
      this->payload = (uint8_t*)&object;
      //Calculate checksum as XOR of all bytes in header and payload
      checksum = 0;
      uint8_t* p = (uint8_t*)&id;
      for(int i=0;i<sizeof(this->id);i++)
        checksum ^= *(p+i);
        
      p = (uint8_t*)&payload_size;
      for(int i=0;i<sizeof(this->payload_size);i++)
        checksum ^= *(p+i);
        
      for(int i=0;i<payload_size;i++)
        checksum ^= *(payload+i);
    };
    
    void send(){
      //Write start byte
      Serial.write((uint8_t)'s');
      //Write Header 
      Serial.write((uint8_t*)&this->id,sizeof(this->id));
      //Serial.write((uint8_t*)(void*)this->id, sizeof(this->id));
      Serial.write((uint8_t*)&this->payload_size,sizeof(this->payload_size));
      //Write payload
      Serial.write(this->payload,this->payload_size);
      //Write checksum
      Serial.write(checksum);
    };
};


#endif

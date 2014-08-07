#include <SoftwareSerial.h>

int state;
int header_size;
int buffer_index;
uint8_t buffer[256];
uint8_t checksum;

uint16_t id;
uint16_t payload_size;

SoftwareSerial ser(10,11);

void setup() {
  Serial.begin(9600);
  ser.begin(9600);
  state = 1;
  header_size = 4;
  buffer_index = 0;
}
void loop() {
  //Wait for start byte
  if(state == 1) {
    buffer_index = 0;
    if(ser.available() > 0) {
      uint8_t b = (uint8_t)ser.read();
      if(b == (uint8_t)'s') {
        state = 2;
      }
    }
  }
  //Read header
  if(state == 2) {
    if(buffer_index < header_size) {
      if(ser.available() > 0) {
        uint8_t b = (uint8_t)ser.read();
        buffer[buffer_index] = b;
        buffer_index++;
      }
    }
    else {
      state = 3;
    }
  }
  //Parse header
  if(state == 3) {
    id = *((uint16_t*)(void*)&buffer[0]);
    payload_size = *(uint16_t*)(void*)&buffer[sizeof(id)];
    //Serial.println(id);
    state = 4;
  }
  if(state == 4) {
    if(buffer_index < (header_size+payload_size)){
      if(ser.available() > 0) {
        uint8_t b = (uint8_t)ser.read();
        buffer[buffer_index] = b;
        buffer_index++;
      }
    }
    else
      state = 5;
  }
  if(state == 5) {
     double f;
    f = *((double*)(void*)&buffer[header_size]);
    Serial.println(f);
    state = 1;
  }
}


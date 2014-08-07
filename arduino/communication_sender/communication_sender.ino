#include <SoftwareSerial.h>
#include "communication.h"

//SoftwareSerial ser(10,11);

//Header h;
Message message;

double f;

void setup() {
  Serial.begin(9600);
  ser.begin(9600);
  f=20.99423;
}
void loop() {
  message.pack(5,f);
  Serial.println(message.size);
  ser.write((uint8_t)'s');
  ser.write(message.data,message.size);
  delay(1000);
}

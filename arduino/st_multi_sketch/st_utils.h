#ifndef ST_UTILS_H_
#define ST_UTILS_H_

//St util includes utilities such as setting an N mux channel
//Multiplexor Function Declarations
void InitNMux(int* pins, int number_of_pins) {
  for (int i=0; i<number_of_pins; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW);
  } 
};
void SelectNMuxChannel(int channel, int* pins, int number_of_pins) {
 for(int i=0;i<number_of_pins;i++)
   digitalWrite(pins[i], (channel>>i)&1);
};

#endif

#include "utilities.h"


//Multiplexor
void InitNMux(int* pins, int number_of_pins) {
  for (int i=0; i<number_of_pins; i++) {
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], LOW);
  } 
}

void SelectNMuxChannel(int channel, int* pins, int number_of_pins) {
 for(int i=0;i<number_of_pins;i++)
   digitalWrite(pins[i], (channel>>i)&1);
}


//Display
void DisplayImuCalibrationData(ImuCalibrationData *d) {
  Serial.print(d->mx_min); Serial.print(", ");
  Serial.print(d->my_min); Serial.print(", ");
  Serial.print(d->mz_min); Serial.print(", ");
  Serial.print(d->mx_max); Serial.print(", ");
  Serial.print(d->my_max); Serial.print(", ");
  Serial.println(d->mz_max);
}

void DisplayImuData(ImuData *d) {
  Serial.print(d->mx); Serial.print(", ");
  Serial.print(d->my); Serial.print(", ");
  Serial.print(d->mz); Serial.print(", ");
  Serial.print(d->ax); Serial.print(", ");
  Serial.print(d->ay); Serial.print(", ");
  Serial.print(d->az); Serial.print(", ");
  Serial.print(d->gx); Serial.print(", ");
  Serial.print(d->gy); Serial.print(", ");
  Serial.println(d->gz); 
}

void DisplayQuaternion(Quaternion *q) {
  Serial.print(q->q0); Serial.print(", ");
  Serial.print(q->q1); Serial.print(", ");
  Serial.print(q->q2); Serial.print(", ");
  Serial.println(q->q3);
}

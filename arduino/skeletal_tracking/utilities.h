#ifndef _UTILITIES_H
#define _UTILITIES_H

//Include Libraries
#include <Arduino.h>
#include "data_structures.h"

//Multiplexor Function Declarations
void InitNMux(int* pins, int number_of_pins);
void SelectNMuxChannel(int channel, int* pins, int number_of_pins);

//Display Function Declarations
void DisplayImuCalibrationData(ImuCalibrationData *d);
void DisplayImuData(ImuData *d);
void DisplayQuaternion(Quaternion *q);

#endif

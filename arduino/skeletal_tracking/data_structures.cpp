#include "data_structures.h"
#include "Arduino.h"

ImuCalibrationData::ImuCalibrationData(float mx_min, float my_min, float mz_min, float mx_max, float my_max, float mz_max){
  this->mx_min = mx_min;
  this->my_min = my_min;
  this->mz_min = mz_min;
  this->mx_max = mx_max;
  this->my_max = my_max;
  this->mz_max = mz_max;
}

ImuData::ImuData(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) {
  this->gx = gx;
  this->gy = gy;
  this->gz = gz;
  this->ax = ax;
  this->ay = ay;
  this->az = az;
  this->mx = mx;
  this->my = my;
  this->mz = mz;
}

Quaternion::Quaternion() {
  this->q0 = 1.0f;
  this->q1 = 0.0f;
  this->q2 = 0.0f;
  this->q3 = 0.0f;
}
Quaternion::Quaternion(float q0, float q1, float q2, float q3) {
  this->q0 = q0;
  this->q1 = q1;
  this->q2 = q2;
  this->q3 = q3;
}




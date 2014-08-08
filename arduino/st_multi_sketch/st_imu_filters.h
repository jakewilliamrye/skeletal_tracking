#ifndef ST_IMU_FILTERS_H_
#define ST_IMU_FILTERS_H_

#include "st_quaternion.h"
#include "st_imu_measurement.h"

//Madgwick 6DOF
void Madgwick6DOF(StQuaternion* q, StImuMeasurement m, float beta, float freq) {
  
};

//Madgwick 9DOF
void Madgwick9DOF(StQuaternion* q, StImuMeasurement m, float beta, float freq) {
  
};

//Mahony 6DOF
void Mahony6DOF(StQuaternion* q, StImuMeasurement m) {
  
};

//Mahony 9DOF
void Mahony9DOF(StQuaternion* q, StImuMeasurement m) {
  
};


#endif

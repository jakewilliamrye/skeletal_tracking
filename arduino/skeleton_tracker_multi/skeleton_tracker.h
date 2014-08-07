#ifndef SKELETON_TRACKER_H_
#define SKELETON_TRACKER_H_

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>
#include "MadgwickAHRS.h"

//Sensor drivers are global objects
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(20);
sensors_event_t event;

typedef struct {
  float q0 = 1, q1 = 0, q2 = 0,q3 = 0;
} Quaternion;

typedef struct {
  float wx,wy,wz,ax,ay,az,mx,my,mz;
} ImuRaw;

typedef struct {
  float mag_min_x, mag_min_y, mag_min_z;
  float mag_max_x, mag_max_y, mag_max_z;
} CalibrationData;

void initializeImus() {
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    //Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    //Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  if(!gyro.begin())
  {
    /* There was a problem detecting the L3GD20 ... check your connections */
    //Serial.print("Ooops, no L3GD20 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
};

void updateQuaternion(Quaternion* q, ImuRaw imu, float beta, float freq) {
  //MadgwickAHRS algorithm here
  MadgwickAHRSupdate(imu.wx,imu.wy,imu.wz,imu.ax,imu.ay,imu.az,imu.mx,imu.my,imu.mz,
               &q->q0, &q->q1, &q->q2, &q->q3, beta, freq);
};

void readImuRaw(ImuRaw* imu) 
{
  //Read accelerometer
  accel.getEvent(&event);
  imu->ax = event.acceleration.x;
  imu->ay = event.acceleration.y;
  imu->az = event.acceleration.z;
  
  //Read magnetometer
  mag.getEvent(&event);
  imu->mx = event.magnetic.x;
  imu->my = event.magnetic.y;
  imu->mz = event.magnetic.z;
  
  //Read gyroscope
  gyro.getEvent(&event);
  imu->wx = event.gyro.x;
  imu->wy = event.gyro.y;
  imu->wz = event.gyro.z;
};

void applyCalibration(ImuRaw* imu, CalibrationData calib)
{
  //Shift the raw imu magnetometer data by the offset.  The calibration offset
  //is the average of the min and max values.  This shifting operation centers the readings
  //at around 0.
  imu->mx -= (calib.mag_min_x + calib.mag_max_x) / 2.0f;
  imu->my -= (calib.mag_min_y + calib.mag_max_y) / 2.0f;
  imu->mz -= (calib.mag_min_z + calib.mag_max_z) / 2.0f;
  
  //Scale the magnetometer readings so each axis reads a value between -1.0f and 1.0f.
  imu->mx *= 2.0f / (calib.mag_max_x - calib.mag_min_x);
  imu->my *= 2.0f / (calib.mag_max_y - calib.mag_min_y);
  imu->mz *= 2.0f / (calib.mag_max_z - calib.mag_min_z);
};

#endif

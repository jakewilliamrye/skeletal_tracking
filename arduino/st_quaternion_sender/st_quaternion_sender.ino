#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>
#include "skeleton_tracker.h"
#include "communication.h"

ImuRaw imu1;
CalibrationData calib1;
Quaternion quaternion{1.0f,0.0f,0.0f,0.0f};
Message msg;

long start_time_us, end_time_us, elapsed_time_us;
const long period_us = 10000;
long convergence_time_start;
long convergence_time_ms = 5000;

void setup() {
  Serial.begin(9600);
  for(int i=0;i<3;i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  
  initializeImus();
  calib1.mag_min_x = -57.55;
  calib1.mag_min_y = -39.36;
  calib1.mag_min_z = -56.22;
  calib1.mag_max_x = 45.00;
  calib1.mag_max_y = 62.73;
  calib1.mag_max_z = 48.27;
  
  convergence_time_start = millis();
}

void loop() {
  start_time_us=micros();
  //DONT WRITE BEFORE HERE
  readImuRaw(&imu1);
  applyCalibration(&imu1,calib1);
  float beta;
  if((millis() - convergence_time_start) > convergence_time_ms)
    beta = 0.1;
  else
    beta = 2.5;
    
  updateQuaternion(&quaternion, imu1, beta, 100);
  
  
  //Send the quaternion
  msg.pack(10,quaternion);
  Serial.write((uint8_t)'s');
  Serial.write(msg.data, msg.size);
  
  //DONT WRITE PAST HERE
  end_time_us=micros();
  elapsed_time_us=end_time_us - start_time_us;
  if (elapsed_time_us < period_us)
    delayMicroseconds(period_us - elapsed_time_us + 200); 
}

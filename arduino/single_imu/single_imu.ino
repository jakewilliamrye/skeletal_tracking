#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>
//#include "MahonyAHRS.h"
#include "MadgwickAHRS.h"
#include "communication.h"

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(20);

float gx,gy,gz,ax,ay,az,mx,my,mz;
float mag_min[3];
float mag_max[3];

Message message;

sensors_event_t event;

long start_time_us, end_time_us, elapsed_time_us;
const long period_us = 10000;

void setup(void)
{
  Serial.begin(115200);
  //Serial.println(F("Adafruit 9DOF Tester")); Serial.println("");
  
  /* Initialise the sensors */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println(F("Ooops, no LSM303 detected ... Check your wiring!"));
    while(1);
  }
  if(!mag.begin())
  {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
  if(!gyro.begin())
  {
    /* There was a problem detecting the L3GD20 ... check your connections */
    Serial.print("Ooops, no L3GD20 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  //Min: 22.36, 24.36, -149.49 Max: 86.36, 73.91, -57.45

  mag_min[0] = 22.36;
  mag_min[1] = 24.36;
  mag_min[2] = -149.49;
  mag_max[0] = 86.36;
  mag_max[1] = 73.91;
  mag_max[2] = -57.45;
  
  //calibrate_magnetometer(); 
}

void loop() {
  start_time_us=micros();
  //DONT WRITE BEFORE HERE
  
  if(Serial.available()>0) {
     char c = Serial.read();
     if(c == 'c')
        calibrate_magnetometer(); 
     if(c == 'b')
       beta = 3.0f;
     if(c == 'd')
       beta = 0.1;
  }
  getMagCalibrated();
  getAccel();
  getGyro();
  MadgwickAHRSupdate(gx, gy, gz,
                     ax, ay, az,
                     mx, my, mz);
  
  //float yaw  = atan2(2*q1*q2 - 2*q0*q3,2*q0*q0 + 2*q1*q1 - 1);
 
  //Serial.println(yaw);
  /*Serial.print(q0);
  Serial.print(" ");
  Serial.print(q1);
  Serial.print(" ");
  Serial.print(q2);
  Serial.print(" ");
  Serial.println(q3);*/
  
  //DONT WRITE PAST HERE
  end_time_us=micros();
  elapsed_time_us=end_time_us - start_time_us;
  if (elapsed_time_us > period_us)
     Serial.println("E"); 
  else
    delayMicroseconds(period_us - elapsed_time_us + 200); 
}

void getMagCalibrated() {
  mag.getEvent(&event);
  //Shift the readings by the offset
  mx = event.magnetic.x - (mag_min[0]+mag_max[0])/2.0f;
  my = event.magnetic.y - (mag_min[1]+mag_max[1])/2.0f;
  mz = event.magnetic.z - (mag_min[2]+mag_max[2])/2.0f;
  //Normalize the readings by scaling by the full range
  mx *= 2.0f / (mag_max[0] - mag_min[0]);
  my *= 2.0f / (mag_max[1] - mag_min[1]);
  mz *= 2.0f / (mag_max[2] - mag_min[2]);
}
void getAccel() {
   accel.getEvent(&event);
   ax = event.acceleration.x;
   ay = event.acceleration.y;
   az = event.acceleration.z;
}
void getGyro() {
  gyro.getEvent(&event);
  gx = event.gyro.x;
  gy = event.gyro.y;
  gz = event.gyro.z;
}
void calibrate_magnetometer() {
  Serial.println("Move the magnetometer in all directions.");
  Serial.println("Enter 'c' to complete calibration");
  float mag_min_temp[3];
  float mag_max_temp[3];
  
  //Get initial mins and maxes
  mag.getEvent(&event);
  mag_min_temp[0] = event.magnetic.x;
  mag_min_temp[1] = event.magnetic.y;
  mag_min_temp[2] = event.magnetic.z;
  mag_max_temp[0] = event.magnetic.x;
  mag_max_temp[1] = event.magnetic.y;
  mag_max_temp[2] = event.magnetic.z;
  
  //Get mins and maxes until escape character is entered
  boolean calibrating = true;
  while(calibrating) 
  {
    mag.getEvent(&event);
    //Update running minimum
    if(event.magnetic.x < mag_min_temp[0])
      mag_min_temp[0] = event.magnetic.x;
    if(event.magnetic.y < mag_min_temp[1])
      mag_min_temp[1] = event.magnetic.y;
    if(event.magnetic.z < mag_min_temp[2])
      mag_min_temp[2] = event.magnetic.z;
      
    //Update running maximum
    if(event.magnetic.x > mag_max_temp[0])
      mag_max_temp[0] = event.magnetic.x;
    if(event.magnetic.y > mag_max_temp[1])
      mag_max_temp[1] = event.magnetic.y;
    if(event.magnetic.z > mag_max_temp[2])
      mag_max_temp[2] = event.magnetic.z;
      
    //Check if we should continue with calibration
    if(Serial.available()>0){
      if(Serial.read()=='c') 
        calibrating = false;
    }
    //Display the current calibration values
    Serial.print("Min: ");
    Serial.print(mag_min_temp[0]);
    Serial.print(", ");
    Serial.print(mag_min_temp[1]);
    Serial.print(", ");
    Serial.print(mag_min_temp[2]);
    Serial.print(" Max: ");
    Serial.print(mag_max_temp[0]);
    Serial.print(", ");
    Serial.print(mag_max_temp[1]);
    Serial.print(", ");
    Serial.println(mag_max_temp[2]);
    delay(100);
  }
  mag_min[0] = mag_min_temp[0];
  mag_min[1] = mag_min_temp[1];
  mag_min[2] = mag_min_temp[2];
  mag_max[0] = mag_max_temp[0];
  mag_max[1] = mag_max_temp[1];
  mag_max[2] = mag_max_temp[2];
  Serial.println("Calibration complete.");
}

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>
//#include "MahonyAHRS.h"
#include "MadgwickAHRS.h"

//ROS includes
#include <ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/tf.h>

Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
Adafruit_L3GD20_Unified       gyro  = Adafruit_L3GD20_Unified(20);

float gx,gy,gz,ax,ay,az,mx,my,mz;
float mag_min[3];
float mag_max[3];

sensors_event_t event;

long start_time_us, end_time_us, elapsed_time_us;
const long period_us = 10000;

long convergence_time_start;
long convergence_time_ms = 5000;

ros::NodeHandle  nh;
geometry_msgs::TransformStamped t;
tf::TransformBroadcaster broadcaster;

char base_link[] = "belt";
char odom[] = "map";

void setup(void)
{
  for(int i=0;i<3;i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  
   //Set up ROS
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  broadcaster.init(nh);
  
  /* Initialise the sensors */
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
  //Min: -54.18, -54.36, -60.20 Max: 50.27, 50.91, 46.12
  //Min: -44.73, -51.36, -44.39 Max: 46.09, 42.55, 45.31

  mag_min[0] = -44.73;
  mag_min[1] = -51.36;
  mag_min[2] = -44.39;
  mag_max[0] = 46.09;
  mag_max[1] = 42.55;
  mag_max[2] = 45.31;    
  
  
  
  convergence_time_start = millis();
  //calibrate_magnetometer(); 
}

void loop() {
  start_time_us=micros();
  //DONT WRITE BEFORE HERE
  if((millis() - convergence_time_start) > convergence_time_ms)
    beta = 0.1;
  else
    beta = 2.5;
    
  getMagCalibrated();
  getAccel();
  getGyro();
  MadgwickAHRSupdate(gx, gy, gz,
                     ax, ay, az,
                     mx, my, mz);
  
  t.header.frame_id = base_link;
  t.child_frame_id = odom;
  t.transform.rotation.x = q1;
  t.transform.rotation.y = q2;
  t.transform.rotation.z = q3;
  t.transform.rotation.w = q0;//= tf::createQuaternion(q1,q2,q3,q0);
  t.header.stamp = nh.now();
  broadcaster.sendTransform(t);
  nh.spinOnce();
  
  //DONT WRITE PAST HERE
  end_time_us=micros();
  elapsed_time_us=end_time_us - start_time_us;
  if (elapsed_time_us < period_us)
    delayMicroseconds(period_us - elapsed_time_us + 200); 
  //   Serial.println("E"); 
  //else
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

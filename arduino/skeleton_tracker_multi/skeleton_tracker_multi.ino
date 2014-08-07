#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>
#include "skeleton_tracker.h"
//ROS includes
#include <ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/tf.h>

ImuRaw imu1;
CalibrationData calib1;
Quaternion quaternion;

ros::NodeHandle  nh;
geometry_msgs::TransformStamped t;
tf::TransformBroadcaster broadcaster;

char base_link[] = "belt";
char odom[] = "map";

long start_time_us, end_time_us, elapsed_time_us;
const long period_us = 10000;
long convergence_time_start;
long convergence_time_ms = 5000;
void setup() {
  //Set up ROS
  nh.getHardware()->setBaud(115200);
  nh.initNode();
  broadcaster.init(nh);
  
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
  /*Serial.print(quaternion.q0);
  Serial.print(", ");
  Serial.print(quaternion.q1);
  Serial.print(", ");
  Serial.print(quaternion.q2);
  Serial.print(", ");
  Serial.println(quaternion.q3);*/
  t.header.frame_id = base_link;
  t.child_frame_id = odom;
  t.transform.rotation.x = quaternion.q1;
  t.transform.rotation.y = quaternion.q2;
  t.transform.rotation.z = quaternion.q3;
  t.transform.rotation.w = quaternion.q0;//= tf::createQuaternion(q1,q2,q3,q0);
  t.header.stamp = nh.now();
  broadcaster.sendTransform(t);
  nh.spinOnce();
  
  //DONT WRITE PAST HERE
  end_time_us=micros();
  elapsed_time_us=end_time_us - start_time_us;
  if (elapsed_time_us < period_us)
    delayMicroseconds(period_us - elapsed_time_us + 200); 
}

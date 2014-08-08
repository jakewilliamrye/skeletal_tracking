#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>
#include "st_imu_calibration.h"
#include "st_quaternion.h"
#include "st_imu_measurement.h"
#include "st_imu_filters.h"
#include "st_imu_configuration.h"
#include "st_imu_driver.h"
#include "st_message_header.h"
#include "st_message.h"
#include "st_message_sender.h"
#include "st_message_router.h"
#include "st_message_receiver.h"

StImuMeasurement m;
StQuaternion q;
StImuCalibration mc;
StImuDriver imu_driver;

void setup () {
  imu_driver.init();
}
void loop () {
  imu_driver.readAll(&m);
  delay(100);
}

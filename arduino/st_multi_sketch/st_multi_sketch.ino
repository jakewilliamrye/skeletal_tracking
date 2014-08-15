#include <aJSON.h>
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
/*#include "st_message_header.h"
#include "st_message.h"
#include "st_message_sender.h"
#include "st_message_router.h"
#include "st_message_receiver.h"
#include "st_utils.h"*/

StImuMeasurement imu_measurement;
StQuaternion q;
StImuCalibration imu_calibration;
StImuDriver imu_driver;
aJsonStream serial_stream(&Serial);
//StMessage message;

void setup () {
  Serial.begin(115200);
  imu_driver.init();
  imu_calibration.calibrateMagnetometer(&imu_driver, 10000, 50);
}
void loop () {
  imu_driver.readAll(&imu_measurement);
  imu_calibration.applyCalibration(&imu_measurement);
  Madgwick9DOF(&q, imu_measurement, 2.5f, 20.0f);
  aJsonObject *msg = aJson.createObject();
  aJson.addStringToObject(msg, "class", "q");
  double qf[4] = {q.q0,q.q1,q.q2,q.q3};
  aJsonObject *aQuaternion = aJson.createFloatArray(qf,4);
  aJson.addItemToObject(msg, "qbelt", aQuaternion);
    aJson.print(msg, &serial_stream);
    Serial.println(); /* Add newline. */
    aJson.deleteItem(msg);
  //message.pack(5,q);
  //Serial.write(message.data, message.size);
  delay(50);
}

#ifndef ST_IMU_DRIVER_H_
#define ST_IMU_DRIVER_H_

//External dependencies
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_9DOF.h>

//Internal dependencies
#include "st_imu_measurement.h"
#include "st_imu_configuration.h"

//StImuDriver provides an interface to the IMU.  It provides methods for reading data into an st_imu_measurement.
class StImuDriver {
  private:
    Adafruit_LSM303_Accel_Unified accel;
    Adafruit_LSM303_Mag_Unified   mag;
    Adafruit_L3GD20_Unified       gyro;
    sensors_event_t event;
  public:
    StImuDriver() {
      this->accel = Adafruit_LSM303_Accel_Unified(30301);
      this->mag = Adafruit_LSM303_Mag_Unified(30302);
      this->gyro = Adafruit_L3GD20_Unified(20);
    };
    //Initializes the accelerometer, gyroscope, and magnetometer.  Returns an integer corresponding to the outcome
    //of the initialization.  The outcome coding is as follows
    //  1 = success
    //  -1 = failed to detect accelerometer
    //  -2 = failed to detect magnetometer
    //  -3 = failed to detect gyroscope
    int init() {
      if(!this->accel.begin())
        return -1;
      if(!this->mag.begin())
        return -2;
      if(!this->gyro.begin())
        return -3;
        
      return 1;
    };
    void readAll(StImuMeasurement* m) {
      //Read the gyroscope
      this->gyro.getEvent(&this->event);
      m->gx = event.gyro.x;
      m->gy = event.gyro.y;
      m->gz = event.gyro.z;
      
      //Read the accelerometer
      this->accel.getEvent(&this->event);
      m->ax = event.acceleration.x;
      m->ay = event.acceleration.y;
      m->az = event.acceleration.z;
      
      //Read the magnetometer
      this->mag.getEvent(&this->event);
      m->mx = event.magnetic.x;
      m->my = event.magnetic.y;
      m->mz = event.magnetic.z;
    };
    void sendConfiguration(StImuConfiguration* c);
};

#endif

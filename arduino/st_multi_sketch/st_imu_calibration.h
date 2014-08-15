#ifndef ST_IMU_CALIBRATION_H_
#define ST_IMU_CALIBRATION_H_

#include <Arduino.h>

#include "st_imu_measurement.h"
#include "st_imu_driver.h"

//StImuCalibration stores calibration data that is aquired from performing a calibration.  This calibration may be applied to an st_imu_measurement.
class StImuCalibration {
  public:
    //VARIABLES
    //Gyroscope axis gains and offsets
    float gx_gain, gy_gain, gz_gain;
    float gx_offset, gy_offset, gz_offset;
    boolean gyro_calibrated;
    
    //Accelerometer axis gains and offsets
    float ax_gain, ay_gain, az_gain;
    float ax_offset, ay_offset, az_offset;
    boolean accel_calibrated;
    
    //Magnetometer axis gains and offsets
    float mx_gain, my_gain, mz_gain;
    float mx_offset, my_offset, mz_offset;
    boolean mag_calibrated;
    
    //METHODS
    StImuCalibration() {
      gyro_calibrated = false;
      accel_calibrated = false;
      mag_calibrated = false;
    };
    int toASCII(char* buffer) {
      
    };
    void applyCalibration(StImuMeasurement* m) {
      m->mx = (m->mx - this->mx_offset) * this->mx_gain;
      m->my = (m->my - this->my_offset) * this->my_gain;
      m->mz = (m->mz - this->mz_offset) * this->mz_gain;
    };
    //calibrateMagnetometer records the min and max values read from the magnetometer
    //over the duration specified by calibration_time_ms.  These values are used to determine the 
    //gains and offsets of each magnetometer axis, which are used to normalize the magnetometer reading
    //of each axis.
    boolean calibrateMagnetometer(StImuDriver* imu_driver, long calibration_time_ms, int sampling_interval_ms) {
      StImuMeasurement m;
      float mx_max, my_max, mz_max;
      float mx_min, my_min, mz_min;
      long calibration_start_time_ms = millis();
      //Initialize mins and maxes
      imu_driver->readAll(&m);
      mx_max = m.mx;
      my_max = m.my;
      mz_max = m.mz;
      mx_min = m.mx;
      my_min = m.my;
      mz_min = m.mz;
      //Search for the highest and lowest values of each axis while the calibration is running
      while((millis() - calibration_start_time_ms) < calibration_time_ms) {
        imu_driver->readAll(&m);
        //Update x axis extrema
        if(m.mx > mx_max)
          mx_max = m.mx;
        if(m.mx < mx_min)
          mx_min = m.mx;
        //Update y axis extrema
        if(m.my > my_max)
          my_max = m.my;
        if(m.my < my_min)
          my_min = m.my;
        //Update z axis extrema
        if(m.mz > mz_max)
          mz_max = m.mz;
        if(m.mz < mz_min)
          mz_min = m.mz;
        
        delay(sampling_interval_ms);
      }
      //Validate that the mins and maxes are not identical, to ensure the calibration is of practical
      //value.  If the calibration is succesful, update the internal values and set calibrated to true.
      if((mx_min==mx_max)||(my_min==my_max)||(mz_min==mz_max))
        return false;
      else {
        //Calculate offsets from min/max values
        this->mx_offset = (mx_max + mx_min) / 2.0f;
        this->my_offset = (my_max + my_min) / 2.0f;
        this->mz_offset = (mz_max + mz_min) / 2.0f;
        //Calculate gains from min/max values
        this->mx_gain = 2.0f / (mx_max - mx_min);
        this->my_gain = 2.0f / (my_max - my_min);
        this->mz_gain = 2.0f / (mz_max - mz_min);
        mag_calibrated = true;
        return true;
      }
    };
};

#endif

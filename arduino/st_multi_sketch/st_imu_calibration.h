#ifndef ST_IMU_CALIBRATION_H_
#define ST_IMU_CALIBRATION_H_

//StImuCalibration stores calibration data that is aquired from performing a calibration.  This calibration may be applied to an st_imu_measurement.
class StImuCalibration {
  public:
    //Gyroscope axis gains and offsets
    float gx_gain, gy_gain, gz_gain;
    float gx_offset, gy_offset, gz_offset;
    
    //Accelerometer axis gains and offsets
    float ax_gain, ay_gain, az_gain;
    float ax_offset, ay_offset, az_offset;
    
    //Magnetometer axis gains and offsets
    float mx_gain, my_gain, mz_gain;
    float mx_offset, my_offset, mz_offset;
};

#endif

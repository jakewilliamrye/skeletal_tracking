#ifndef ST_IMU_MEASUREMENT_H_
#define ST_IMU_MEASUREMENT_H_


//StImuMeasurement contains the angular rate, magnetic, and acceleration
//vectors in SI units.
class StImuMeasurement {
  public:
    float gx, gy, gz;
    float mx, my, mz;
    float ax, ay, az;
};

#endif

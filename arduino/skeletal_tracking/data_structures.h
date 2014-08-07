#ifndef _DATA_STRUCTURES_H_
#define _DATA_STRUCTURES_H_

//Structure Declarations
class ImuCalibrationData {
  public:
    //Public Variables
    float mx_min, my_min, mz_min;
    float mx_max, my_max, mz_max;
    //Public Methods
    ImuCalibrationData(){};
    ImuCalibrationData(float mx_min, float my_min, float mz_min, float mx_max, float my_max, float mz_max); 
};
  
class ImuData {
  public:
    //Public Variables
    float mx, my, mz;
    float ax, ay, az;
    float gx, gy, gz;
    //Public Methods
    ImuData(){};
    ImuData(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
};

class Quaternion {
  public:
    //Public Variables
    float q0, q1, q2, q3;
    //Public Methods
    Quaternion();
    Quaternion(float q0, float q1, float q2, float q3);
};

//Function Declarations - Initialization
void InitImuCalibrationData (ImuCalibrationData *d);
void InitImuDataRaw(ImuData *d);
void InitImuDataCorrected(ImuData *d);
void InitQuaternion(Quaternion *q);

//Function Declarations - Display


//Function Declarations - Normal Operation
void AcquireImuCalibrationData(ImuCalibrationData *d);
void ReadImuDataRaw(ImuData *d);
void GetImuDataCorrected(ImuData *corrected, ImuData *raw, ImuCalibrationData *c);
void UpdateQuaternion(Quaternion *quaternion, ImuData *corrected);

#endif

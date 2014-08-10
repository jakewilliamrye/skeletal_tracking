#ifndef ST_ROTATION_H_
#define ST_ROTATION_H_

#include <Arduino.h>

//StQuaternion class
class StQuaternion {
  public:
    StQuaternion() {
      this->q0 = 1.0f;
      this->q1 = 0.0f;
      this->q2 = 0.0f;
      this->q3 = 0.0f;
    };
    float q0, q1, q2, q3;
};

#endif

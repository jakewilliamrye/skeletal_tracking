#ifndef ST_IMU_FILTERS_H_
#define ST_IMU_FILTERS_H_

#include "st_quaternion.h"
#include "st_imu_measurement.h"

float invSqrt(float x);

//Madgwick 6DOF
void Madgwick6DOF(StQuaternion* q, StImuMeasurement m, float beta, float freq) {
  float recipNorm;
  float s0, s1, s2, s3;
  float qDot1, qDot2, qDot3, qDot4;
  float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

  // Rate of change of quaternion from m.gyroscope
  qDot1 = 0.5f * (-q->q1 * m.gx - q->q2 * m.gy - q->q3 * m.gz);
  qDot2 = 0.5f * (q->q0 * m.gx + q->q2 * m.gz - q->q3 * m.gy);
  qDot3 = 0.5f * (q->q0 * m.gy - q->q1 * m.gz + q->q3 * m.gx);
  qDot4 = 0.5f * (q->q0 * m.gz + q->q1 * m.gy - q->q2 * m.gx);

  // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
  if(!((m.ax == 0.0f) && (m.ay == 0.0f) && (m.az == 0.0f))) {

    // Normalise accelerometer measurement
    recipNorm = invSqrt(m.ax * m.ax + m.ay * m.ay + m.az * m.az);
    m.ax *= recipNorm;
    m.ay *= recipNorm;
    m.az *= recipNorm;   

    // Auxiliary variables to avoid repeated arithmetic
    _2q0 = 2.0f * q->q0;
    _2q1 = 2.0f * q->q1;
    _2q2 = 2.0f * q->q2;
    _2q3 = 2.0f * q->q3;
    _4q0 = 4.0f * q->q0;
    _4q1 = 4.0f * q->q1;
    _4q2 = 4.0f * q->q2;
    _8q1 = 8.0f * q->q1;
    _8q2 = 8.0f * q->q2;
    q0q0 = q->q0 * q->q0;
    q1q1 = q->q1 * q->q1;
    q2q2 = q->q2 * q->q2;
    q3q3 = q->q3 * q->q3;

    // Gradient decent algorithm corrective step
    s0 = _4q0 * q2q2 + _2q2 * m.ax + _4q0 * q1q1 - _2q1 * m.ay;
    s1 = _4q1 * q3q3 - _2q3 * m.ax + 4.0f * q0q0 * q->q1 - _2q0 * m.ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * m.az;
    s2 = 4.0f * q0q0 * q->q2 + _2q0 * m.ax + _4q2 * q3q3 - _2q3 * m.ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * m.az;
    s3 = 4.0f * q1q1 * q->q3 - _2q1 * m.ax + 4.0f * q2q2 * q->q3 - _2q2 * m.ay;
    recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
    s0 *= recipNorm;
    s1 *= recipNorm;
    s2 *= recipNorm;
    s3 *= recipNorm;

    // Apply feedback step
    qDot1 -= beta * s0;
    qDot2 -= beta * s1;
    qDot3 -= beta * s2;
    qDot4 -= beta * s3;
  }

  // Integrate rate of change of quaternion to yield quaternion
  q->q0 += qDot1 * (1.0f / freq);
  q->q1 += qDot2 * (1.0f / freq);
  q->q2 += qDot3 * (1.0f / freq);
  q->q3 += qDot4 * (1.0f / freq);

  // Normalise quaternion
  recipNorm = invSqrt(q->q0 * q->q0 + q->q1 * q->q1 + q->q2 * q->q2 + q->q3 * q->q3);
  q->q0 *= recipNorm;
  q->q1 *= recipNorm;
  q->q2 *= recipNorm;
  q->q3 *= recipNorm;
};

//Madgwick 9DOF
void Madgwick9DOF(StQuaternion* q, StImuMeasurement m, float beta, float freq) {
  float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float hx, hy;
	float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;

	// Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
	if((m.mx == 0.0f) && (m.my == 0.0f) && (m.mz == 0.0f)) {
		Madgwick6DOF(q, m, beta, freq);
		return;
	}

	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-q->q1 * m.gx - q->q2 * m.gy - q->q3 * m.gz);
	qDot2 = 0.5f * (q->q0 * m.gx + q->q2 * m.gz - q->q3 * m.gy);
	qDot3 = 0.5f * (q->q0 * m.gy - q->q1 * m.gz + q->q3 * m.gx);
	qDot4 = 0.5f * (q->q0 * m.gz + q->q1 * m.gy - q->q2 * m.gx);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if(!((m.ax == 0.0f) && (m.ay == 0.0f) && (m.az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(m.ax * m.ax + m.ay * m.ay + m.az * m.az);
		m.ax *= recipNorm;
		m.ay *= recipNorm;
		m.az *= recipNorm;   

		// Normalise magnetometer measurement
		recipNorm = invSqrt(m.mx * m.mx + m.my * m.my + m.mz * m.mz);
		m.mx *= recipNorm;
		m.my *= recipNorm;
		m.mz *= recipNorm;

		// Auxiliary variables to avoid repeated arithmetic
		_2q0mx = 2.0f * q->q0 * m.mx;
		_2q0my = 2.0f * q->q0 * m.my;
		_2q0mz = 2.0f * q->q0 * m.mz;
		_2q1mx = 2.0f * q->q1 * m.mx;
		_2q0 = 2.0f * q->q0;
		_2q1 = 2.0f * q->q1;
		_2q2 = 2.0f * q->q2;
		_2q3 = 2.0f * q->q3;
		_2q0q2 = 2.0f * q->q0 * q->q2;
		_2q2q3 = 2.0f * q->q2 * q->q3;
		q0q0 = q->q0 * q->q0;
		q0q1 = q->q0 * q->q1;
		q0q2 = q->q0 * q->q2;
		q0q3 = q->q0 * q->q3;
		q1q1 = q->q1 * q->q1;
		q1q2 = q->q1 * q->q2;
		q1q3 = q->q1 * q->q3;
		q2q2 = q->q2 * q->q2;
		q2q3 = q->q2 * q->q3;
		q3q3 = q->q3 * q->q3;

		// Reference direction of Earth's magnetic 
		hx = m.mx * q0q0 - _2q0my * q->q3 + _2q0mz * q->q2 + m.mx * q1q1 + _2q1 * m.my * q->q2 + _2q1 * m.mz * q->q3 - m.mx * q2q2 - m.mx * q3q3;
		hy = _2q0mx * q->q3 + m.my * q0q0 - _2q0mz * q->q1 + _2q1mx * q->q2 - m.my * q1q1 + m.my * q2q2 + _2q2 * m.mz * q->q3 - m.my * q3q3;
		_2bx = sqrt(hx * hx + hy * hy);
		_2bz = -_2q0mx * q->q2 + _2q0my * q->q1 + m.mz * q0q0 + _2q1mx * q->q3 - m.mz * q1q1 + _2q2 * m.my * q->q3 - m.mz * q2q2 + m.mz * q3q3;
		_4bx = 2.0f * _2bx;
		_4bz = 2.0f * _2bz;

		// Gradient decent algorithm corrective step
		s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - m.ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - m.ay) - _2bz * q->q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - m.mx) + (-_2bx * q->q3 + _2bz * q->q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - m.my) + _2bx * q->q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - m.mz);
		s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - m.ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - m.ay) - 4.0f * q->q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - m.az) + _2bz * q->q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - m.mx) + (_2bx * q->q2 + _2bz * q->q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - m.my) + (_2bx * q->q3 - _4bz * q->q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - m.mz);
		s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - m.ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - m.ay) - 4.0f * q->q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - m.az) + (-_4bx * q->q2 - _2bz * q->q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - m.mx) + (_2bx * q->q1 + _2bz * q->q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - m.my) + (_2bx * q->q0 - _4bz * q->q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - m.mz);
		s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - m.ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - m.ay) + (-_4bx * q->q3 + _2bz * q->q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - m.mx) + (-_2bx * q->q0 + _2bz * q->q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - m.my) + _2bx * q->q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - m.mz);
		recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= beta * s0;
		qDot2 -= beta * s1;
		qDot3 -= beta * s2;
		qDot4 -= beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	q->q0 += qDot1 * (1.0f / freq);
	q->q1 += qDot2 * (1.0f / freq);
	q->q2 += qDot3 * (1.0f / freq);
	q->q3 += qDot4 * (1.0f / freq);

	// Normalise quaternion
	recipNorm = invSqrt(q->q0 * q->q0 + q->q1 * q->q1 + q->q2 * q->q2 + q->q3 * q->q3);
	q->q0 *= recipNorm;
	q->q1 *= recipNorm;
	q->q2 *= recipNorm;
	q->q3 *= recipNorm;
};

//Mahony 6DOF
void Mahony6DOF(StQuaternion* q, StImuMeasurement m) {
  
};

//Mahony 9DOF
void Mahony9DOF(StQuaternion* q, StImuMeasurement m) {
  
};

//---------------------------------------------------------------------------------------------------
// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root

float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
};

#endif

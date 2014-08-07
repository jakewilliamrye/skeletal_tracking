//Include Libraries
#include<EEPROM.h>
#include "data_structures.h"
#include "utilities.h"
#include "madgwick_ahrs.h"
#include "serial_communication.h"

//Declare Structures
ImuCalibrationData imu_calibration_data;
ImuData imu_data_raw, imu_data_corrected; 
Quaternion quaternion;

//Declare Variables
int mux_channel_select_pins[]={0,1,2,3}; //LSB->MSB (s0,s1,s2,..sN)
int mux_number_pins=4;

//Setup
void setup(){
  //Initialize Serial Port
  Serial.begin(9600);
}

//Loop
Message m;
int count=1;
float val=0;
void loop() {
  val=(float)(count%10)/10;
  quaternion.q0=val;
  quaternion.q1=1-val;
  
  m.pack(10,quaternion);
  m.send();
  delay(1);
  count++;
}

void DisplayDataStructures() {
  Serial.print("imu calibration data: "); DisplayImuCalibrationData(&imu_calibration_data);
  Serial.print("imu data raw: "); DisplayImuData(&imu_data_raw);
  Serial.print("imu data corected: "); DisplayImuData(&imu_data_corrected);
  Serial.print("quaternion: "); DisplayQuaternion(&quaternion);   
}


void BarebonesCommunication(){
  uint8_t start_byte='s';
  uint16_t payload_id=1024;
  uint16_t num_payload_bytes=3;
  uint8_t payload[] = {10,12,14};
  uint8_t checksum=0;
  
  Serial.write(start_byte);
  Serial.write(payload_id>>8);
  Serial.write(payload_id);
  Serial.write(num_payload_bytes>>8);
  Serial.write(num_payload_bytes);
  for (int i=0; i<num_payload_bytes; i++) 
    Serial.write(payload[i]);
  Serial.write(checksum); 
}




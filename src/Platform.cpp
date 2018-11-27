#include "Arduino.h"
#include "Platform.h"
#include "MPU9250.h"

MPU9250 IMU9250(Wire,0x68);
int stat;

int START_SOLENOID = 2;
int TOTAL_SOLENOIDS = 8;
bool solenoids[8];


Platform::Platform(){
	
}

bool Platform::initialize(void){
	stat = IMU9250.begin();
	if(stat < 0) return false;
	return true;
	
	IMU9250.setAccelRange(MPU9250::ACCEL_RANGE_8G);
  // setting the gyroscope full scale range to +/-500 deg/s
  IMU9250.setGyroRange(MPU9250::GYRO_RANGE_500DPS);
  // setting DLPF bandwidth to 20 Hz
  IMU9250.setDlpfBandwidth(MPU9250::DLPF_BANDWIDTH_20HZ);
  // setting SRD to 19 for a 50 Hz update rate
  IMU9250.setSrd(19);
	
	for(int i = 0; i < TOTAL_SOLENOIDS; i++){
		solenoids[i] = false;
		pinMode(i + START_SOLENOID, OUTPUT);
	}	
}

bool Platform::isSolenoidActive(int num){
	return solenoids[num];
}

void Platform::setSolenoidActive(int num, bool active){
	solenoids[num] = active;
	if(active){
		digitalWrite(num + START_SOLENOID, HIGH);
		Serial.print("Enabled: ");
		Serial.println(num + START_SOLENOID);
	}else{
		digitalWrite(num + START_SOLENOID, LOW);
  	Serial.print("Disabled: ");
		Serial.println(num + START_SOLENOID);
	}
}
void Platform::readData(void){
	IMU9250.readSensor();
}

void Platform::getAccel(Vector* vector){
	vector->x = IMU9250.getAccelX_mss();
	vector->y = IMU9250.getAccelY_mss();
	vector->z = IMU9250.getAccelZ_mss();
}

void Platform::getGyro(Vector* vector){
	vector->x = IMU9250.getGyroX_rads();
	vector->y = IMU9250.getGyroY_rads();
	vector->z = IMU9250.getGyroZ_rads();
}

void Platform::getMag(Vector* vector){
	vector->x = IMU9250.getMagX_uT();
	vector->y = IMU9250.getMagY_uT();
	vector->z = IMU9250.getMagZ_uT();
}


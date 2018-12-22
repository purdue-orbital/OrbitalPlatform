#include "Arduino.h"
#include "Platform.h"
#include "MPU9250.h"

MPU9250 IMU9250(Wire,0x68);
int stat;

int START_SOLENOID = 2;
int TOTAL_SOLENOIDS = 8;

bool solenoids[8] = {0};
int* solenoidPins; // Array of solenoid pins

Platform::Platform(){ // Default Constructor. Uses pins START_SOLENOID to START_SOLENOID + TOTAL_SOLENOIDS
	solenoidPins = (int*) malloc(sizeof(int) * TOTAL_SOLENOIDS);
	for(int i = 0; i < TOTAL_SOLENOIDS; i++){
		solenoidPins[i] = START_SOLENOID + i;
	}
}

Platform::Platform(int* pins, int length){ // Constructor using given pins
	solenoidPins = pins;
	TOTAL_SOLENOIDS = length;
}

bool Platform::initialize(void){
	stat = IMU9250.begin();
	if(stat < 0) return false;
	

	
	IMU9250.setAccelRange(MPU9250::ACCEL_RANGE_16G);
		
	for(int i = 0; i < TOTAL_SOLENOIDS; i++){
		solenoids[i] = false;
		//pinMode(i + START_SOLENOID, OUTPUT);
		pinMode(solenoidPins[i], OUTPUT);
	}	

  return true;
	
}

void Platform::printPins(){
	Serial.print("Pins: {");
	for(int i = 0; i < TOTAL_SOLENOIDS - 1; i++){
		Serial.print(solenoidPins[i]);
		Serial.print(", ");
	}
	Serial.print(solenoidPins[TOTAL_SOLENOIDS - 1]);
	Serial.println("}");
}

bool Platform::isSolenoidActive(int num){
	return solenoids[num-1];
}

void Platform::setSolenoidActive(int num, bool active){
	
	if(solenoids[num-1] == active) return;
	
	solenoids[num-1] = active;
	
	if(active){
		digitalWrite(solenoidPins[num-1], HIGH);
		Serial.print("Enabled: ");
		Serial.println(solenoidPins[num-1]);
	}else{
		digitalWrite(solenoidPins[num-1], LOW);
  	Serial.print("Disabled: ");
		Serial.println(solenoidPins[num-1]);
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


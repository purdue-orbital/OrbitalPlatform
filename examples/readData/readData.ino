/*
	readData
	
	Reads data from a MPU9250, prints gyroscope data to console, and turns a solenoid is it is rotating > 0.5 rads/s

*/


#include <Platform.h>

Platform plat = Platform();

void setup(){
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.println("Initializing...");
	
	bool success = plat.initialize();
	
	if(!success){
		do{
			Serial.println("Error: IMU Initialization Failed. Retrying...");
			success = plat.initialize();
			delay(5000);
		}while(success == false);
	}
	Serial.println("IMU Enabled");
	digitalWrite(LED_BUILTIN, HIGH);	 // Enable built in LED to show that the device is enabled properly
}

Vector accel;  // m/s^2
Vector gyro;   // rads/s
Vector mag;    // uT

void loop(){
	plat.readData();
	
	plat.getAccel(&accel);
	plat.getGyro(&gyro);
	plat.getMag(&mag);

	Serial.print(gyro.x, 6);
	Serial.print(" ");
	Serial.print(gyro.y, 6);
	Serial.print(" ");
	Serial.print(gyro.z, 6);
	Serial.print("\n");

	if(gyro.z > 0.5){ // If the device is rotating clockwise @ more than 0.5 rads/s turn on the solenoid
		plat.setSolenoidActive(4, true);
	}else{
		plat.setSolenoidActive(4, false);
	}

	delay(50);
}
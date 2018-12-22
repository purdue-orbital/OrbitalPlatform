/*
	readData
	
	Reads data from a MPU9250, prints gyroscope data to console, and turns a 

*/
#include <Platform.h>

//int pins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
int pins[3] = {4, 5, 6};

//Platform plat = Platform(pins, 8);
Platform plat = Platform(pins, 3);

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
	plat.printPins();
	digitalWrite(LED_BUILTIN, HIGH);	 // Enable built in LED to show that the device is enabled properly
}

Vector accel;	// m/s^2
Vector gyro;	 // rads/s
Vector mag;		// uT

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
	
	
	if(gyro.z < -0.5){ // If the device is rotating counter-clockwise @ more than 0.5 rads/s turn on solenoid 1 (pin 4)
		plat.setSolenoidActive(1, true);

		plat.setSolenoidActive(2, false);
		plat.setSolenoidActive(3, false);
		
	}else if(gyro.z > 0.5){ // If the device is rotating clockwise @ more than 0.5 rads/s turn on solenoid 2 (pin 6)
		plat.setSolenoidActive(3, true);
		
		plat.setSolenoidActive(1, false);
		plat.setSolenoidActive(2, false);
	}else{ // If the device is rotating less than 0.5 rads/s turn on solenoid 2 (pin 5) NOTE: This is connected to a LED instead of an actual solenoid
		plat.setSolenoidActive(2, true);
		
		plat.setSolenoidActive(1, false);
		plat.setSolenoidActive(3, false);
		
	} 

	delay(50);
}
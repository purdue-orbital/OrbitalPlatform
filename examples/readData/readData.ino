/*
	readData

	Reads data from a MPU9250, prints gyroscope data to console, and turns a

*/
#include "Platform.h"
#include <SPI.h>
#include <SD.h>

void enableIMU();
void enableSD();
void filePrint();
void printVector(Vector v);
//int pins[8] = {2, 3, 4, 5, 6, 7, 8, 9};
int pins[8] = {2, 3, 5, 6, 7, 8, 9, 10};

//Platform plat = Platform(pins, 8);
Platform plat = Platform(pins, 8);

File file;

void setup(){
	Serial.begin(115200);
	pinMode(LED_BUILTIN, OUTPUT);

    pinMode(A0, OUTPUT); //Error: piezo buzzer/LED



	enableIMU();
	enableSD();

	int i = 0;
	while(SD.exists("data" + String(i) + ".txt")){
		i++;
	}

	file = SD.open("data" + String(i) + ".txt", FILE_WRITE);

	//plat.printPins();
}


void enableSD(){

	if(!SD.begin(4)){
		while(1){
			digitalWrite(A0, HIGH);
			delay(500);
			digitalWrite(A0, LOW);
			delay(500);
		}
	}

}


void enableIMU(){
	Serial.println("Initializing IMU");
	bool success = plat.initialize();
	if(!success){
		do{
			Serial.println("Error: IMU Initialization Failed. Retrying...");
			success = plat.initialize();
			digitalWrite(A0, HIGH);
			delay(5000);
		}while(success == false);
	}
	digitalWrite(A0, LOW);
	//Serial.println("IMU Enabled");
}

Vector accel;	// m/s^2
Vector gyro;	 // rads/s
Vector mag;		// uT



void filePrint(){
	file.print(millis());
	file.print(',');
	printVector(accel);
	file.print(',');
	printVector(gyro);
	file.print(',');
	printVector(mag);
	file.println();
}

void printVector(Vector v){
	file.print(v.x);
	file.print(',');
	file.print(v.y);
	file.print(',');
	file.print(v.z);
}

long lastTime = millis();

void loop(){

	if(lastTime + 200 < millis()){
		filePrint();
		file.flush();
		lastTime = millis();
		return;
	}

	plat.readData();

	plat.getAccel(&accel);
	plat.getGyro(&gyro);
	plat.getMag(&mag);

	//Serial.print(gyro.x, 6);
	//Serial.print(" ");
	//Serial.print(gyro.y, 6);
	//Serial.print(" ");
	//Serial.print(gyro.z, 6);
	//Serial.print("\n");



	if(gyro.z < -0.25){ // If the device is rotating counter-clockwise @ more than 0.5 rads/s turn on solenoid 1 (pin 4)
		plat.setSolenoidActive(1, true);
		plat.setSolenoidActive(2, true);
		plat.setSolenoidActive(3, true);
		plat.setSolenoidActive(4, true);

		plat.setSolenoidActive(5, false);
		plat.setSolenoidActive(6, false);
		plat.setSolenoidActive(7, false);
		plat.setSolenoidActive(8, false);


		//Serial.println("Right");
	}else if(gyro.z > 0.25){ // If the device is rotating clockwise @ more than 0.5 rads/s turn on solenoid 2 (pin 6)
		plat.setSolenoidActive(1, false);
		plat.setSolenoidActive(2, false);
		plat.setSolenoidActive(3, false);
		plat.setSolenoidActive(4, false);

		plat.setSolenoidActive(5, true);
		plat.setSolenoidActive(6, true);
		plat.setSolenoidActive(7, true);
		plat.setSolenoidActive(8, true);
	}else{ // If the device is rotating less than 0.5 rads/s turn on solenoid 2 (pin 5) NOTE: This is connected to a LED instead of an actual solenoid
		plat.setSolenoidActive(1, false);
		plat.setSolenoidActive(2, false);
		plat.setSolenoidActive(3, false);
		plat.setSolenoidActive(4, false);
		plat.setSolenoidActive(5, false);
		plat.setSolenoidActive(6, false);
		plat.setSolenoidActive(7, false);
		plat.setSolenoidActive(8, false);

	}

}

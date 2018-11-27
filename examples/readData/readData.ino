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
     delay(1000);
		}while(success == false);
	}
  Serial.println("IMU Enabled");
  digitalWrite(LED_BUILTIN, HIGH);   // Enable built in LED to show that the device is enabled properly
}

Vector accel; // m/s^2
Vector gyro;  // rads/s
Vector mag;   // uT

void loop(){
	plat.readData();
	
	plat.getAccel(&accel);
	plat.getGyro(&gyro);
	plat.getMag(&mag);

  Serial.print(accel.x, 6);
  Serial.print(" ");
  Serial.print(accel.y, 6);
  Serial.print(" ");
  Serial.print(accel.z, 6);
  Serial.print("\n");

  if(gyro.z > 0.5){ // If the device is rotating clockwise @ more than 0.5 rads/s turn on the solenoid
    plat.setSolenoidActive(4, true);
  }else{
    plat.setSolenoidActive(4, false);
  }

  delay(50);
	digitalWrite(LED_BUILTIN, HIGH);
  //plat.setSolenoidActive(3, true);
	delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  //plat.setSolenoidActive(3, false);
}
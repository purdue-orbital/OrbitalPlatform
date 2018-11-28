#include <Platform.h>

#define CAL_POINT (200) //Number of calibration points for the gyroscope
#define C_CLOCKWISE (0.3) //rad/s
#define CLOCKWISE (-0.3) //rad/s

#define CLOCKWISE_PUSH (170) //8-bit representation of clockwise solenoids
#define C_CLOCKWISE_PUSH (85) //8-bit representation of counter-clockwise solenoids
#define CLOSE_ALL (0) //Used to close all solenoids

Platform plat = Platform();

Vector accel; // m/s^2
Vector gyro;  // rad/s
Vector mag;   // uT

Vector accel_cal = {0.0, 0.0, 0.0};
Vector gyro_cal = {0.0, 0.0, 0.0};
Vector mag_cal = {0.0, 0.0, 0.0};

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.println("Initializing...");
  
  bool success = plat.initialize();
  
  while(!success){
    Serial.println("Error: IMU Initialization Failed. Retrying...");
    success = plat.initialize();
    delay(5000);
  }

  calibrate_gyro(); //Ensure the IMU is not moving or rotating during calibration
  
  Serial.println("IMU Enabled");
  digitalWrite(LED_BUILTIN, HIGH); 
}

void loop() {
  plat.readData();
  
  plat.getAccel(&accel);
  plat.getMag(&mag);
  plat.getGyro(&gyro);
  sub_vector(&gyro, &gyro_cal);

  printData();
  if (gyro.z > C_CLOCKWISE) {
    activateSolenoids(CLOCKWISE_PUSH);
    //LED1
  }
  else if (gyro.z < CLOCKWISE) {
    activateSolenoids(C_CLOCKWISE_PUSH);
    //LED2
  }
  else {
    activateSolenoids(CLOSE_ALL);
  }
}

void printData() {
  Serial.print("Gyro (rad/s): ");
  Serial.print(gyro.x, 6);
  Serial.print(" ");
  Serial.print(gyro.y, 6);
  Serial.print(" ");
  Serial.print(gyro.z, 6);
  Serial.print("\n");
}

void activateSolenoids(unsigned char code) {
  //TODO
  
  //Each of the 8 bits in the argument represent a solenoid
  //Bits with value 1 open the corresponding solenoid
  //Bits with value 0 close the corresponding solenoid

  unsigned char on = 1;

  for (int i = 0; i < TOTAL_SOLENOIDS; i++) {
    if ((code & on) == on) {
      plat.setSolenoid(i, true);
    }
    else {
      plat.setSolenoid(i, false);
    }
    on = on << 1;
  }
}

void calibrate_gyro() {
  for (int i = 0; i < CAL_POINTS; i++) {
    plat.readData();
    plat.getGyro(&gyro);
    gyro_cal.x += gyro.x;
    gyro_cal.y += gyro.y;
    gyro_cal.z += gyro.z;
  }
  gyro_cal.x /= CAL_POINTS;
  gyro_cal.y /= CAL_POINTS;
  gyro_cal.z /= CAL_POINTS;
}

void sub_vector(Vector *v1, Vector *v2) {
  v1->x -= v2->x;
  v1->y -= v2->y;
  v1->z -= v2->z;
}

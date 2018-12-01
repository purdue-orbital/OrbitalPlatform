///////////////////////////////////////////////////////////
//
//  Top view of the platform control system. Each number
//  corresponds to a solenoid. The arrow indicates the
//  direction of air flow when the solenoid is open. The
//  IMU should be placed at the center of the cross.
//
//                <-1 2->
//        ^         | |         ^
//        |         | |         |
//        8---------------------3
//        7---------------------4
//        |         | |         |
//        v         | |         v
//                <-6 5->
//
///////////////////////////////////////////////////////////


#include <Platform.h>

#define CAL_POINTS (200) //Number of calibration points for the gyroscope
#define CLOCKWISE_MAX (-0.3) //rad/s
#define C_CLOCKWISE_MAX (0.3) //rad/s
#define TOTAL_SOLENOIDS (3) //TEMPORARY. Switch back to 8 after testing

#define CLOCKWISE_PUSH (85) //8-bit representation of clockwise solenoids
#define C_CLOCKWISE_PUSH (170) //8-bit representation of counter-clockwise solenoids
#define CLOSE_ALL (0) //Used to close all solenoids

int pins[8] = {4, 6, 5};

Platform plat = Platform(pins, 3);

Vector accel; // m/s^2
Vector gyro;  // rad/s
Vector mag;   // uT

Vector gyro_cal = {0.0, 0.0, 0.0}; //Offset when the IMU is still

/*  Set up the IMU, calibrate the gyroscope, print the
 *  initial offset to the serial output.
 */

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
  Serial.println("IMU Enabled");
  digitalWrite(LED_BUILTIN, HIGH); 

  Serial.println("Calibrating..");
  calibrate_gyro(); //Ensure the IMU is not moving or rotating during calibration
  Serial.print("Done. Offsets: ");
  Serial.print(gyro_cal.x);
  Serial.print(", ");
  Serial.print(gyro_cal.y);
  Serial.print(", ");
  Serial.print(gyro_cal.z);
  Serial.print("\n");
}

/*  Read all the data from the IMU. Adjust the gyroscope according
 *  to the calibration. Active solenoids depending on the
 *  rotation of the platform.
 */

void loop() {
  plat.readData();
  
  plat.getAccel(&accel);
  plat.getMag(&mag);
  plat.getGyro(&gyro);
  sub_vector(&gyro, &gyro_cal);

  printData();
  if (gyro.z > C_CLOCKWISE_MAX) {
    activateSolenoids(CLOCKWISE_PUSH);
  }
  else if (gyro.z < CLOCKWISE_MAX) {
    activateSolenoids(C_CLOCKWISE_PUSH);
  }
  else {
    activateSolenoids(CLOSE_ALL);
  }
}

/*  Print out the gyroscope data from the accelerometer
 *  to 6 figures.
 */

void printData() {
  Serial.print("Gyro (rad/s): ");
  Serial.print(gyro.x, 6);
  Serial.print(" ");
  Serial.print(gyro.y, 6);
  Serial.print(" ");
  Serial.print(gyro.z, 6);
  Serial.print("\n");
}


/*  This function turns on a set of solenoids. Each of the 8 bits 
 *  in the argument represent a solenoid. Bits with value 1 open
 *  the corresponding solenoid. Bits with value 0 close the 
 *  corresponding solenoid.
 */
void activateSolenoids(unsigned char code) {
  //TODO: Requires further testing
 
  unsigned char on = 1;

  for (int i = 1; i <= TOTAL_SOLENOIDS; i++) {
    if ((code & on) == on) {
      plat.setSolenoidActive(i, true);
    }
    else {
      plat.setSolenoidActive(i, false);
    }
    on = on << 1; //advance to the next solenoid
  }
}

/*  Take a large number of data points from the gyroscope.
 *  Calculate the average value to detemine the offset, if
 *  any. The IMU must be still during the calibration
 *  of the gyroscope.
 */

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

/*  Subtract each element of the two vectors
 */

void sub_vector(Vector *v1, Vector *v2) {
  v1->x -= v2->x;
  v1->y -= v2->y;
  v1->z -= v2->z;
}

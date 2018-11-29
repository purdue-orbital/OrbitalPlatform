#ifndef Platform_h
#define Platform_h

#include "Arduino.h"

		typedef struct{
			double x;
			double y;
			double z;
		}Vector;	

class Platform{
	public:
		Platform();
		Platform::Platform(int* pins, int length);
		bool initialize();
		void getAccel(Vector*);
		void getGyro(Vector* vector);
		void getMag(Vector* vector);
		void readData();
		bool isSolenoidActive(int num);
		void setSolenoidActive(int num, bool active);			
};

#endif
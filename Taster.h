/*
 Taster.h - Library for handling the door's button
 */

#ifndef Taster_h
#define Taster_h

#include<Arduino.h>

class Taster{
public:
	Taster(int pin);
	void loopTaster();
protected:
	int _pin;
};

#endif


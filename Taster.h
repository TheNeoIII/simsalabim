/*
 Taster.h - Library for handling the door's button
 */

#ifndef Taster_h
#define Taster_h

#include<Arduino.h>

class Taster{
  public:
  	Taster(byte, byte);
  	bool isPressed();
  protected:
  	byte _pin;
    byte _mode;
};

#endif


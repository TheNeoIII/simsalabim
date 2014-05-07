/*
 * Taster.cpp
 *
 *  Created on: 06.05.2014
 *      Author: leonid
 */

#include "Taster.h"

Taster::Taster(byte pin, byte mode) {
  _pin = pin;
  _mode = mode;
  pinMode(pin, mode);
}

bool Taster::isPressed(){
	return !digitalRead(_pin);
}

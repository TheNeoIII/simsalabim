/*
 * Taster.cpp
 *
 *  Created on: 06.05.2014
 *      Author: leonid
 */

#include "Taster.h"

Taster::Taster(int pin) {
	// TODO Auto-generated constructor stub
	_pin = pin;
	pinMode(pin, INPUT);
	Serial.println("[Taster] Setup complete");
}
bool Taster::getStatus(){
	return !digitalRead(_pin);
}

/*
 * Relais.cpp
 *
 *  Created on: 06.05.2014
 *      Author: leonid
 */

#include "Relais.h"

Relais::Relais(int pin) {
	// TODO Auto-generated constructor stub
	_pin = pin;
	pinMode(pin, OUTPUT);
	_status = false;
}

void Relais::close() {
	Serial.println(F("[relais] closing."));
	digitalWrite(_pin, LOW);
	_status = false;
}

void Relais::open() {
	Serial.println(F("[relais] opening!"));
	digitalWrite(_pin, HIGH);
	_status = true;
}

bool Relais::getStatus(){
	return _status;
}


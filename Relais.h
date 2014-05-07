/*
 Relais.h - Library for handling the relais control
 */

#ifndef Relais_h
#define Relais_h

#include <Arduino.h>

class Relais{
public:
	Relais(int pin);
	void open();
	void close();
	bool getStatus();
protected:
	int _pin;
	bool _status;
};

#endif


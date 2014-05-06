/*
 * Watchdog.h
 *
 *  Created on: 06.05.2014
 *      Author: leonid
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include<Arduino.h>
#include <avr/wdt.h>

class Watchdog {
public:
	Watchdog();
	void reset();
};

#endif /* WATCHDOG_H_ */

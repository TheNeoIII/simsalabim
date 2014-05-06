/*
 * Watchdog.cpp
 *
 *  Created on: 06.05.2014
 *      Author: leonid
 */

#include "Watchdog.h"

Watchdog::Watchdog() {
	// TODO Auto-generated constructor stub
	/*** Setup the WDT ***/

	/* Clear the reset flag. */
	MCUSR &= ~(1 << WDRF);

	WDTCSR = (1 << WDCE) | (1 << WDE);
	WDTCSR = (1 << WDP0) | (1 << WDP3); //8sec until reset
	WDTCSR |= (1 << WDIE);
}

void Watchdog::reset(){
	wdt_reset();
}


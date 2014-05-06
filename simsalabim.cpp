/*** Simsalabim Tueroeffnung ***/

#include <Arduino.h>

#include "Relais.h"
#include "Storage.h"
#include "RFID.h"
#include "Taster.h"
#include "Watchdog.h"

Relais relais(3); 	//relais on pin 3
Taster taster(4); 	//taster on pin 4
RFID rfid(9, 10); 	//rfid on chipselect pin 9 and reset pin 10
Watchdog wdt;		//watchdog for reset after 8 sec

unsigned long timerDoor = 0;
byte *uid;

//Setup in class constructor
void setup() {
	Serial.begin(9600);
}

void loop() {
	//Open Door if timerDoor is set
	if (timerDoor > millis()) {
		relais.openDoor();
	} else {
		relais.closeDoor();
	}

	//Open Door for 3 Seconds if Taster pressed
	if (taster.getStatus()) {
		timerDoor = millis() + 3000;
	}

	//Open Door if RFID Card Authenticated
	if (rfid.isCardAvailable()) {
		uid = rfid.getCurrentKey();
		//use storage class for auth check
		//if(){
		timerDoor = millis() + 3000;
		//}
	}

	wdt.reset();
}


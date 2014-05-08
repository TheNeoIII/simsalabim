/*** Simsalabim Tueroeffnung ***/

#include <SPI.h>
#include <SD.h>

#include "Watchdog.h"
#include "Relais.h"
#include "Storage.h"
#include "RFIDModul.h"
#include "Taster.h"
#include "EthernetModul.h"


Watchdog wdt;		//watchdog for reset after 8 sec
Relais door(3); 	//relais on pin 3
Taster taster(4, INPUT); 	//taster on pin 4
RFIDModul rfid(10, 9); 	//rfid on chipselect pin 9 and reset pin 10
Storage storage(7);	//sd card module with chipselect on pin 7
//EthernetModul network(8); //cs on pin 8

unsigned long timerDoor = 0;
byte uid[5];

//Setup in class constructor
void setup() {
	Serial.begin(9600);
	Serial.println("Setup Complete");
}

void loop() {
	//Open Door if timerDoor is set
	//open door if not already open
	if (timerDoor > millis() && !door.getStatus()) {
		door.open();
	} else if(timerDoor < millis() && door.getStatus()){
		//close door if open
		door.close();
	}

	//Open Door for 3 Seconds if Taster pressed
	if (taster.isPressed()) {
		timerDoor = millis() + 1500;
	}

	//Open Door if RFID Card Authenticated
	//Only works if module present

	if (rfid.isCardAvailable()) {
		rfid.getCurrentKey();
		//use storage class for auth check

		if(storage.findKey(rfid.currentUID) != KEY_NOT_FOUND){
			timerDoor = millis() + 1500;
		}
	}

	wdt.reset();
}


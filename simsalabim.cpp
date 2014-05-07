/*** Simsalabim Tueroeffnung ***/

#include <SPI.h>
#include <SD.h>

#include "Watchdog.h"
#include "Relais.h"
#include "Storage.h"
//#include "RFID.h"
#include "Taster.h"


Watchdog wdt;		//watchdog for reset after 8 sec
Relais door(3); 	//relais on pin 3
Taster taster(4, INPUT); 	//taster on pin 4
//RFID rfid(9, 10); 	//rfid on chipselect pin 9 and reset pin 10


unsigned long timerDoor = 0;
byte *uid;

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
		timerDoor = millis() + 3000;
	}

	//Open Door if RFID Card Authenticated
	//Only works if module present
	/*if (rfid.isCardAvailable()) {
		uid = rfid.getCurrentKey();
		//use storage class for auth check
		//if(){
		timerDoor = millis() + 3000;
		//}
	}*/

	wdt.reset();
}


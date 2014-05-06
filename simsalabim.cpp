/*** Simsalabim Tueroeffnung ***/

#include <Arduino.h>

#include "Relais.h"
#include "Storage.h"
#include "RFID.h"
#include "Taster.h"
#include "Watchdog.h"

Relais 	relais(3); 	//relais on pin 3
Taster 	taster(4); 	//taster on pin 4
RFID 	rfid(9, 10);//rfid on chipselect pin 9 and reset pin 10
Watchdog wdt;		//watchdog for reset after 8 sec

//Setup in class constructor
void setup(){
  Serial.begin(9600);
}

void loop()
{

  
	wdt.reset();
}



//#include <TimerOne.h>

/*
Read RFID UID, Store UID, Switch Relais to open door for 3 sec
Normal Mode: Search for UID in EEPROM, Switch relais if present
Adding Mode: Search for UID in EEPROM, if not present, add
Delete Mode: Search for UID in EEPROM, if present, delete
*/

#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <avr/wdt.h>
#include <EtherCard.h>
#include <stdio.h>
#include <SD.h>
  
#include "sdcard.h"
#include "relais.h"
#include "taster.h"
#include "eepromkey.h"
#include "masterkeys.h"
#include "rfid.h"
#include "webserver.h"


void setup(){
  Serial.begin(9600);
  Serial.println("[main] Start setup.");
  
  /*** Setup the WDT ***/
  
  /* Clear the reset flag. */
  MCUSR &= ~(1<<WDRF);
  
  
  WDTCSR = (1<<WDCE) | (1<<WDE) ;
  WDTCSR = (1<<WDP0) | (1<<WDP3); //8sec until reset
  WDTCSR |= (1<<WDIE);
  
  

  //setupRFID(); // Initialize the RFID module
  setupTaster();
  setupRelais();
  setupSDCard();
  setupWebserver();
  
  
  //EEPROM.write(keyCounter, 0x00); //Reset counter

  Serial.println("[main] simsalabim setup complete.");
}

void loop()
{
        loopSDCard();
	loopTaster(); //Handle taster
	loopRelais();  // Handle the door state (close it if needed)
        loopWebserver();
        
	//loopRFID();  // Check if a key is present and handle it
        wdt_reset(); //prevent restart if everything ok
}



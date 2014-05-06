/*** Simsalabim Tueroeffnung ***/

#include <Arduino.h>
#include <SPI.h>
#include <avr/wdt.h>
#include <MFRC522.h>
#include <EEPROM.h>

#include <SD.h>
//#include "sdcard.h"

#include "Relais.h"
#include "Storage.h"
#include "RFID.h"
#include "Taster.h"

//#include "libraries/webserver.h"

void setupWatchdog();

void setup(){
  Serial.begin(9600);
  Serial.println(F("[setup] *** Simsalabim ***"));
  SD.begin();
  SPI.begin();

  setupWatchdog();
  setupRFID(); // Initialize the RFID module
  setupTaster();
  setupRelais();
  //setupSDCard();
  //setupWebserver();
  
  
  //EEPROM.write(keyCounter, 0x00); //Reset counter

  Serial.println(F("[setup] complete."));
}

void loop()
{
  loopTaster(); // Handle the door opener
  loopRelais(); // Handle the door state (close it if needed)
  loopRFID();   // Check if a key is present and handle it

  //loopWebserver();
  
  wdt_reset();  // reset watchdog timer
}

void setupWatchdog()
{
  /*** Setup the WDT ***/

  /* Clear the reset flag. */
  MCUSR &= ~(1<<WDRF);
  
  WDTCSR = (1<<WDCE) | (1<<WDE) ;
  WDTCSR = (1<<WDP0) | (1<<WDP3); //8sec until reset
  WDTCSR |= (1<<WDIE);
}

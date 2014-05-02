//#include <TimerOne.h>

/*
Read RFID UID, Store UID, Switch Relais to open door for 3 sec
Normal Mode: Search for UID in EEPROM, Switch relais if present
Adding Mode: Search for UID in EEPROM, if not present, add
Delete Mode: Search for UID in EEPROM, if present, delete
*/

#include <EEPROM.h>
#include <SPI.h>
#include "TimerOne.h"
#include "rfid.h"
#include "eeprom-key.h"
#include "relais.h"
#include "master-keys.h"
#include "taster.h"

const int NOT_FOUND = -1;

const byte
  pinTaster = 7,
  pinRelais = 3,
  pinRouter = 4,
  pinRST    = 9,
  pinSS     = 10,
  
  stateDoor = false; //False = closed, true = opened
  stateProg	= false; //programming mode on or off

  keySize   = 10,   // num of bytes used to identify the key
  storageAddress = 100;  //storage start position for keys

unsigned long
  openUntil,
  addUntil;

// Prototypes for the functions called from inside the setup routine
void setupRFID();
void setupPINS();

void setup(){
  Serial.begin(9600);

  setupRFID(); // Initialize the RFID module
  setupTaster();
  setupRelais();
  
  //EEPROM.write(keyCounter, 0x00); //Reset counter

  Serial.println("simsalabim setup complete.");
}

void loop()
{
	loopTaster(); //Handle taster
	loopRelais();  // Handle the door state (close it if needed)
	loopMode();  // Handle the mode timing
	loopRFID();  // Check if a key is present and handle it
}



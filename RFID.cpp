/*
 * RFID.cpp
 *
 *  Created on: 06.05.2014
 *      Author: leonid
 */

#include "RFID.h"

const byte masterUID[10] = { 0x14, 0x72, 0x95, 0x5B, 0x00};

RFID::RFID(int pinCS, int pinRS) {
	keySize = 5;
	_rfid = new RFIDLib(pinCS, pinRS);
	Serial.begin(9600);
	SPI.begin();
	_rfid->init();
	Serial.println("[RFID] Ready");
}

void RFID::resetRFID() {
	_rfid->reset();
}

bool RFID::isCardAvailable(){
	return (_rfid->isCard() && _rfid->readCardSerial());
}

byte *RFID::getCurrentKey(){
	// Normalize the card's UID to 10 bytes
	memset(_currentUID, 0, keySize);
	memcpy(_currentUID, _rfid->serNum, keySize);

	dumpUID();

	return _currentUID;
}

void RFID::dumpUID() {
	Serial.print(F("[rfid] current card's ID: "));
	for (byte i = 0; i < keySize; i++) {
		Serial.print(_currentUID[i] < 0x10 ? " 0" : " ");
		Serial.print(_currentUID[i], HEX);
	}
	Serial.println();
}

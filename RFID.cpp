/*
 * RFID.cpp
 *
 *  Created on: 06.05.2014
 *      Author: leonid
 */

#include "RFID.h"
#include "MFRC522.h"

const byte masterUID[10] = { 0x14, 0x72, 0x95, 0x5B, 0x00};

RFID::RFID(int pinCS, int pinRS) {
	// TODO Auto-generated constructor stub
	_pinCS = pinCS;
	_pinRS = pinRS;
	keySize = 5;

	_rfid = new MFRC522(_pinCS, _pinRS);

	pinMode(_pinCS, OUTPUT);

	digitalWrite(_pinCS, LOW);
	_rfid->PCD_Init();
	digitalWrite(_pinCS, HIGH);
}


void RFID::resetRFID() {
	_rfid->PCD_Reset();
}

bool RFID::isCardAvailable(){
	return (_rfid->PICC_IsNewCardPresent() && _rfid->PICC_ReadCardSerial());
}

int *RFID::getCurrentKey(){
	// Normalize the card's UID to 10 bytes
	memset(_currentUID, 0, 5);
	memcpy(_currentUID, _rfid->uid.uidByte, _rfid->uid.size);

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

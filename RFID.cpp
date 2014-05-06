/*
 * RFID.cpp
 *
 *  Created on: 06.05.2014
 *      Author: leonid
 */

#include "RFID.h"
#include "MFRC522.h"

const byte masterUID[10] = { 0x14, 0x72, 0x95, 0x5B, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00 };

unsigned long timeMaster = 0, timeRFID = 0;



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

void RFID::loopRFID() {
	if (timeRFID < millis()) {
		timeRFID = millis() + 500; // wait 500ms before next scan

		digitalWrite(_pinCS, LOW);
		bool foundCard = searchRFID();
		digitalWrite(_pinCS, HIGH);

		if (!foundCard) {
			return;
		}

		// Normalize the card's UID to 10 bytes
		memset(currentUID, 0, 5);
		memcpy(currentUID, _rfid->uid.uidByte, _rfid->uid.size);

		dumpUID();

		if (isMasterKey()) {
			Serial.println("[rfid] master key detected");

			timeMaster = millis() + 3000;
		} else if (timeMaster > 0 && timeMaster > millis()) {
			Serial.println("[rfid] handling next key after master key");

			//saveKey();
			timeMaster = 0;
		}

		/*else if (Storage::findKey() != KEY_NOT_FOUND) {
			//Relais::openDoor();
		}
		*/
	}
}

void RFID::dumpUID() {
	Serial.print(F("[rfid] current card's ID: "));
	for (byte i = 0; i < keySize; i++) {
		Serial.print(currentUID[i] < 0x10 ? " 0" : " ");
		Serial.print(currentUID[i], HEX);
	}
	Serial.println();
}

bool RFID::isMasterKey() {
	for (byte i = 0; i < 10; i++) {
		if (masterUID[i] != currentUID[i]) {
			return false;
		}
	}
	return true;
}

bool RFID::searchRFID() {
	if (!_rfid->PICC_IsNewCardPresent()) {
		return false;
	}

	// Select one of the cards
	if (!_rfid->PICC_ReadCardSerial()) {
		return false;
	}

	return true;
}

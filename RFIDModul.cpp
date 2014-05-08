/*
 * RFIDModul.cpp
 *
 *  Created on: 06.05.2014
 *      Author: leonid
 */

#include "RFIDModul.h"

const byte masterUID[10] = { 0x14, 0x72, 0x95, 0x5B, 0x00};

RFIDModul::RFIDModul(int pinCS, int pinRS) {
	//_rfid = new RFID(pinCS, pinRS);
	_rfid = new MFRC522(pinCS, pinRS);
	Serial.begin(9600);
	SPI.begin();
	//_rfid->init();
	_rfid->PCD_Init();
	Serial.println("[RFIDModul] Ready");
}

void RFIDModul::reset() {
	//_rfid->reset();
	//_rfid->PCD_Reset();
}

bool RFIDModul::isCardAvailable(){
	//return (_rfid->isCard() && _rfid->readCardSerial());
	return (_rfid->PICC_IsNewCardPresent() && _rfid->PICC_ReadCardSerial());
}

void RFIDModul::getCurrentKey(){
	// Normalize the card's UID to 10 bytes
	memset(currentUID, 0, KEY_SIZE);
	memcpy(currentUID, _rfid->uid.uidByte, _rfid->uid.size);
	//memcpy(currentUID, _rfid->serNum, KEY_SIZE);

	dumpUID();
}

void RFIDModul::dumpUID() {
	Serial.print(F("[RFIDModul] current card's ID: "));
	for (byte i = 0; i < KEY_SIZE; i++) {
		Serial.print(currentUID[i] < 0x10 ? " 0" : " ");
		Serial.print(currentUID[i], HEX);
	}
	Serial.println();
}

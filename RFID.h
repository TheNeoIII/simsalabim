/*
 RFID.h - Library for handling the RFID
 */
#ifndef RFID_h
#define RFID_h

#include<Arduino.h>
#include <SPI.h>
#include<MFRC522.h>

#define KEY_NOT_FOUND -1

class RFID {
public:
	RFID(int pinCS, int pinRS);
	void resetRFID();
	bool isCardAvailable();
	bool isModuleAvailable();
	byte *getCurrentKey();
	void dumpUID();
	int keySize;
private:
	MFRC522* _rfid;
	byte _currentUID[5];
};
#endif


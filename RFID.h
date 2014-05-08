/*
 RFID.h - Library for handling the RFID
 */
#ifndef RFID_h
#define RFID_h

#include<Arduino.h>
#include<SPI.h>
#include<RFIDLib.h>

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
	RFIDLib* _rfid;
	byte _currentUID[5];
};
#endif


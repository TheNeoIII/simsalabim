/*
 RFID.h - Library for handling the RFID
 */
#ifndef RFIDModul_h
#define RFIDModul_h

#include<Arduino.h>
#include<SPI.h>
#include<RFID.h>

class RFIDModul {
public:
	RFIDModul(int pinCS, int pinRS);
	void reset();
	bool isCardAvailable();
	bool isModuleAvailable();
	byte *getCurrentKey();
	void dumpUID();
	int keySize;
private:
	RFID* _rfid;
	byte _currentUID[5];
};
#endif


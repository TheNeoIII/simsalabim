/*
 RFID.h - Library for handling the RFID
 */
#ifndef RFIDModul_h
#define RFIDModul_h

#include<Arduino.h>
#include<SPI.h>
//#include<RFID.h>
#include <MFRC522.h>

#define KEY_SIZE 10

class RFIDModul {
public:
	RFIDModul(int pinCS, int pinRS);
	void reset();
	bool isCardAvailable();
	bool isModuleAvailable();
	void getCurrentKey();
	void dumpUID();
	byte currentUID[KEY_SIZE];
private:
	//RFID* _rfid;
	MFRC522* _rfid;

};
#endif


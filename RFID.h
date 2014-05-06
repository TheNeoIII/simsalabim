/*
 RFID.h - Library for handling the RFID
 */
#ifndef RFID_h
#define RFID_h

#include<Arduino.h>
#include<MFRC522.h>

#define KEY_NOT_FOUND -1

class RFID {
public:
	RFID(int pinCS, int pinRS);
	void resetRFID();
	void loopRFID();
	void dumpUID();
	bool isMasterKey();
	bool searchRFID();
	int keySize;
private:
	int _pinCS;
	int _pinRS;
	MFRC522* _rfid;
	int currentUID[5];
};
#endif


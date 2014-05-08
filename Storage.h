/*
 Storage.h - Library for handling the EEPROM storage
 */
#ifndef Storage_h
#define Storage_h

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#define PIN_SS 4
#define FILENAME_KEYS "KEYS.TXT"
#define KEY_NOT_FOUND -1
#define KEY_SIZE 10
#define FRAME_SIZE KEY_SIZE + 1 + (24*7)/8 + 1

class Storage {
public:
	Storage(byte chipSelect);
	long findKey(byte (&)[KEY_SIZE]);
	void saveKey(byte (&)[KEY_SIZE], byte (&)[21]);
	void deleteKey(byte (&)[KEY_SIZE]);

private:
	byte _key[KEY_SIZE];
	byte _pin;
	File _file;

	bool openFile(uint8_t);
	void closeFile();
};

#endif

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

class Storage
{
  public:
    void setup();
    long findKey(byte (&)[5]);
    void saveKey(byte (&)[5], byte (&)[21]);
    void deleteKey(byte (&)[5]);

  private:
    unsigned char
      buffer[28];
    byte
      key[5];
    File
      file;


    bool openFile(uint8_t);
    void closeFile();
};

#endif

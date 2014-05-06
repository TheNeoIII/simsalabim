/*
  Storage.h - Library for handling the EEPROM storage
*/

#ifndef Storage_h
#define Storage_h

#include <Arduino.h>
#include <EEPROM.h>

const int KEY_NOT_FOUND = -1;

byte
  keySize   = 10,   // num of bytes used to identify the key
  storageAddress = 100,  //storage start position for keys
  counterAddress = 0, //Storage location for key counter - max 255 keys
  currentUID[10];

int getKeyCount();
int findKey();
void saveKey();
void deleteKey();


// Returns how many keys are in storage
int getKeyCount(){
   return EEPROM.read(counterAddress);
}

// Returns the number of the current key or KEY_NOT_FOUND if no key matches
int findKey() {
  int address;
  boolean matching;

  byte keyCount = getKeyCount();

  // Iterate over all stored keys
  for (byte i=0; i<keyCount; i++) {
    matching = true;
    address = storageAddress + (keySize*i);

    // Compare the stored key with the received one - as soon as we get
    // a missmatching byte, continue with the next one
    for(byte j=0; j<keySize; j++) {
      if (EEPROM.read(address + j) != currentUID[j]) {
        matching = false;
        break;
      }
    }

    if (matching)
    {
      Serial.print(F("[find] Identified key "));
      Serial.println(i);
      return i;
    }
  }

  Serial.println(F("[find] No stored key matches"));
  return KEY_NOT_FOUND;
}

void saveKey() {
  if (findKey() != KEY_NOT_FOUND) {
    Serial.println(F("[save] Won't save key - already present"));
    return;
  }

  int address = storageAddress + getKeyCount() * keySize;

  // Save each byte of the key
  for(int i=0; i<keySize; i++) {
    EEPROM.write(address + i, currentUID[i]);
  }

  // Increment key counter
  EEPROM.write(counterAddress, getKeyCount() + 1);
}

void deleteKey(){
  int key = findKey();
  if (key == KEY_NOT_FOUND) {
    Serial.println(F("[delete] Won't delete key - not present in storage"));
    return;
  }

  int keyCount = getKeyCount();

  int address = storageAddress + key * keySize;
  int lastAddress = storageAddress + (keyCount - 1) * keySize;

  // Remove key - zero it if its the last in storage,
  // otherwise pull last key to the current key's position
  for(int i=0;i<keySize;i++) {
    if(address != lastAddress){
      EEPROM.write(address + i, EEPROM.read(lastAddress + 1));
      EEPROM.write(lastAddress + 1, 0);
    } else
    {
      EEPROM.write(address + i, 0);
    }
  }

  // decrement key counter
  EEPROM.write(counterAddress, keyCount - 1);
}

#endif

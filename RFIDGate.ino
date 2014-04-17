//#include <TimerOne.h>

/*
Read RFID UID, Store UID, Switch Relais to open door for 3 sec
Normal Mode: Search for UID in EEPROM, Switch relais if present
Adding Mode: Search for UID in EEPROM, if not present, add
Delete Mode: Search for UID in EEPROM, if present, delete
*/

#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>

const int NOT_FOUND = -1;

const byte
  pinTaster = 7,
  pinRelais = 3,
  pinRouter = 4,
  pinRST    = 9,
  pinSS     = 10,

  masterUID[10] = {0x14,0x72,0x95,0x5B,0x00,0x00,0x00,0x00,0x00,0x00}, //Programming chip to add new cards

  keySize   = 10,   // num of bytes used to identify the key
  storageAddress = 100;  //storage start position for keys

boolean
  spiBegan,
  stateAdd,
  stateDel;

MFRC522 rfid(pinSS, pinRST);

byte
  counterAddress = 0, //Storage location for key counter - max 255 keys
  currentUID[10];

unsigned long
  openUntil,
  addModeUntil;

void setupRFID()
{
  // Init SPI bus
  SPI.begin();

  // Init MFRC522 module
  rfid.PCD_Init();
}

void resetRFID()
{
  rfid.PCD_Reset();
}

void setupPINS()
{
  //EEPROM.write(keyCounter, 0x00); //Reset counter

  //setup taster input
  pinMode(pinTaster, INPUT_PULLUP);

  //relais output
  pinMode(pinRelais, OUTPUT);
  digitalWrite(pinRelais, LOW);
}


void setup(){
	//Debug output
	Serial.begin(9600);

  setupRFID();
  setupPINS();
}

void doorTimer(){  //Called from interrupt timerone
   digitalWrite(pinRelais, LOW);
   //Timer1.detachInterrupt();
}

int getKeyCount(){
   return EEPROM.read(counterAddress);
}


// Returns the number of the current key or NOT_FOUND if no key matches
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
  return NOT_FOUND;
}

void saveKey() {
  if (findKey() == NOT_FOUND) {
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

  addModeUntil = 0;
}

void deleteKey(){
  int key = findKey();
  if (key == NOT_FOUND) {
    Serial.println(F("[delete] Won't delete key - not present in DB"));
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

void openDoor(){
  Serial.println(F("[door] opening!"));
  digitalWrite(pinRelais, HIGH);
  openUntil = millis()+1500;
}

void doorLoop()
{
  // Close the door if it has been open long enough
  if(openUntil > 0 && millis() > openUntil){

    digitalWrite(pinRelais, LOW);
    openUntil = 0;

    // Reset the RFID module after each usage
    setupRFID();
  }
}

void modeLoop()
{
  // Switch back to normal mode if speical mode timer is reached
  if(addModeUntil > 0 && millis() > addModeUntil){
    addModeUntil = 0;

    // Reset the RFID module after each usage
    setupRFID();
  }
}

void loop()
{
  doorLoop();  // Handle the door state (close it if needed)
  modeLoop();  // Handle the mode timing
  rfidLoop();  // Check if a key is present and handle it

  // Check if the Taster is being pressed
  if(digitalRead(pinTaster) == LOW){
    openDoor();
  }
}

void rfidLoop()
{
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent()) {
      return;
  }

  // Select one of the cards
  if (!rfid.PICC_ReadCardSerial()) {
       return;
  }

  //Reset uid to zero
  memset(currentUID, 0, 10);

  // Dump UID
  Serial.print(F("[rfid] found card "));

  for (byte i = 0; i < rfid.uid.size && i < keySize; i++) {
       Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
       Serial.print(rfid.uid.uidByte[i], HEX);
       currentUID[i] = rfid.uid.uidByte[i];
  }
  Serial.println();

  // Handle the found key accordingly
  if (isMasterKey()) {
    addModeUntil = millis()+1500;
  } else if (addModeUntil > 0)
  {
    saveKey();
  } else if(findKey() != NOT_FOUND) {
    openDoor();
  }
}


boolean isMasterKey(){
  for (byte i=0; i<10; i++) {
    if(masterUID[i] != currentUID[i])
    {
      return false;
    }
  }
  return true;
}


/*
  RFID.h - Library for handling the RFID
*/
#ifndef RFID_h
#define RFID_h

#define RFID_SS 10
#define RFID_RST 9

const byte
  masterUID[10] = {0x14,0x72,0x95,0x5B,0x00,0x00,0x00,0x00,0x00,0x00};

unsigned long
  timeMaster = 0,
  timeRFID = 0;

void setupRFID();
void resetRFID();
void loopRFID();
void dumpUID();

boolean isMasterKey();
boolean searchRFID();

MFRC522 rfid(RFID_SS, RFID_RST);

void setupRFID(){
  pinMode(RFID_SS, OUTPUT);
  
  digitalWrite(RFID_SS, LOW);
  rfid.PCD_Init();
  digitalWrite(RFID_SS, HIGH);
}

void resetRFID(){
  rfid.PCD_Reset();
}

void loopRFID(){
  boolean foundCard = false;
  if (timeRFID < millis())
  {
    timeRFID = millis() + 500; //wait 500ms before next scan

    digitalWrite(RFID_SS, LOW);
    foundCard = searchRFID();
    digitalWrite(RFID_SS, HIGH);

    if (!foundCard) {
      return;
    }

    // Normalize the card's UID to 10 bytes
    memset(currentUID, 0, 10);
    memcpy(currentUID, rfid.uid.uidByte, rfid.uid.size);
    
    dumpUID();
    
    if (isMasterKey())
    {
      Serial.println("[rfid] master key detected");

      timeMaster = millis() + 3000;
    } else if (timeMaster > 0 && timeMaster > millis())
    {
      Serial.println("[rfid] handling next key after master key");

      saveKey();
      timeMaster = 0;
    } else if(findKey() != KEY_NOT_FOUND)
    {
      openDoor();
    }
  }
}

void dumpUID() {
  Serial.print(F("[rfid] current card's ID: "));
  for (byte i = 0; i < keySize; i++) {
    Serial.print(currentUID[i] < 0x10 ? " 0" : " ");
    Serial.print(currentUID[i], HEX);
  }
  Serial.println();
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

byte searchRFID()
{
  if (!rfid.PICC_IsNewCardPresent()) {
    return 0;
  }

  // Select one of the cards
  if (!rfid.PICC_ReadCardSerial()) {
    return 0;
  }

  return 1;
}
#endif


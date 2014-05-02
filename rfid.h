#include <MFRC522.h>
MFRC522 rfid(pinSS, pinRST);

//rfid.h
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

void loopRFID()
{
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent()) {
      return;
  }

  // Select one of the cards
  if (!rfid.PICC_ReadCardSerial()) {
       return;
  }

  // Normalize the card's UID to 10 bytes
  memset(currentUID, 0, 10);
  memcpy(currentUID, rfid.uid.uidByte, rfid.uid.size);

  dumpUID();

  // Handle the found key accordingly
  if (isMasterKey()) {
    addUntil = millis()+1500;
  } else if (addUntil > 0)
  {
    saveKey();
    addUntil = 0;
  } else if(findKey() != NOT_FOUND) {
    openDoor();
  }
}

#define pinSS 10
#define pinRST 9

byte statScan = false;
        
unsigned long timeRFID = 0;

MFRC522 rfid(pinSS, pinRST);

void dumpUID();

//rfid.h
void setupRFID(){
	// Init SPI bus
	SPI.begin();
	// Init MFRC522 module
	rfid.PCD_Init();
}

void resetRFID(){
	rfid.PCD_Reset();
}

void loopRFID(){
        if(timeRFID < millis() && statScan == false){
          statScan = true;
        }
	if(statScan){
                digitalWrite(6, LOW);//enable rfid module
		//setupRFID();
                resetRFID();
		
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
                
                digitalWrite(6, HIGH);//disable rfid module

                loopMasterKeys();

		// Handle the found key accordingly
		if (statProg){
			saveKey();
                        dataString += "[rfid] key saved";
		} else if(findKey() != NOT_FOUND) {
			openDoor();
                        dataString += "[rfid] key opened door";
		}
                statScan = false;
                timeRFID = millis() + 500; //wait 500ms before next scan
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

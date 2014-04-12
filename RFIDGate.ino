#include <TimerOne.h>

/*
Read RFID UID, Store UID, Switch Relais to open door for 3 sec
Normal Mode: Search for UID in EEPROM, Switch relais if present
Adding Mode: Search for UID in EEPROM, if not present, add
Delete Mode: Search for UID in EEPROM, if present, delete
*/

#include <EEPROM.h>
#include <SPI.h>
#include <MFRC522.h>



#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);        // Create MFRC522 instance.

int keyCounter 	= 0; 	//Storage location for key counter - max 255 keys
int keySize 	= 10; 	//num of storage blocks for one uid
int keyStart 	= 100; 	//storage start position for keys
int opMode	= 0;	//0=normal,1=add,2=del
int pinAdd	= 8;
int pinDel	= 9;
int pinTaster   = 7;
int pinRelais   = 3;
int pinRouter   = 4;
boolean stateAdd;
boolean stateDel;
boolean stateRouter;
boolean stateTaster;
int uid[10];
int startPos;
int doorSeconds = 3;

boolean isOpen = false;
unsigned long openUntil;

void setup(){
	//Debug output
	Serial.begin(115200);
	
	//Setup rfid module
	SPI.begin();               // Init SPI bus
        mfrc522.PCD_Init();        // Init MFRC522 card
    
	//Setup adding mode switch
	pinMode(pinAdd, INPUT_PULLUP);
	
	//Setup delete mode switch
	pinMode(pinDel, INPUT_PULLUP);

        //EEPROM.write(keyCounter, 0x00); //Reset counter
        
        //setup taster input
        pinMode(pinTaster, INPUT_PULLUP);
        
        //router input
        pinMode(pinRouter, INPUT_PULLUP);
        
        //relais output
        pinMode(pinRelais, OUTPUT);
        digitalWrite(pinRelais, LOW);
}

void doorTimer(){  //Called from interrupt timerone
   digitalWrite(pinRelais, LOW); 
   Timer1.detachInterrupt();
}

int numEntries(){
   return EEPROM.read(keyCounter);
}

boolean searchUID(){
  boolean retVal = false;
  int entries = numEntries();
  int i,j = 0;
  int res;
  int matchBlocks;
  for(i=0;i<entries;i++){ //look on each key
    matchBlocks = 0;
    startPos = keyStart + (keySize*i);
    for(j=0;j<keySize;j++){     //compare each block of 5
      if(EEPROM.read(startPos + j) == uid[j]){ 
        matchBlocks++;
      }
    }
    
    if(matchBlocks == keySize){
      return true;
    }
    
  }
  
  return retVal;
}

int posUID(){
  int retKey = 0;
  int entries = numEntries();
  int i,j = 0;
  int res;
  int matchBlocks;
  for(i=0;i<entries;i++){ //look on each key
    matchBlocks = 0;
    startPos = keyStart + (keySize*i);
    for(j=0;j<keySize;j++){     //compare each block of 5
      if(EEPROM.read(startPos + j) == uid[j]){ 
        matchBlocks++;
      }
    }
    
    if(matchBlocks == keySize){
      return i;
    }
    
  }
  
  return retKey;
}

void saveUID(){
  Serial.println("Add");
  if(!searchUID()){ //Save when not in database
    startPos = keyStart + (numEntries() * keySize);
    for(int i=0;i<keySize;i++){
      EEPROM.write(startPos + i, uid[i]);
    }
    EEPROM.write(keyCounter, numEntries() + 1); //Increment key counter
  }
}
void deleteUID(){
  Serial.println("Del");
  if(searchUID()){
    //Search position
    int keyPos = posUID();
    int keyCount = numEntries();
    Serial.println(keyPos);
    //remove on current position
    startPos = keyStart + (keyPos * keySize);
    for(int i=0;i<keySize;i++){
        EEPROM.write(startPos + i, 0);
    }
    //if not last key, move last key into empty space
    if((keyPos + 1) < keyCount){
      int lastKeyStart = keyStart + ((keyCount - 1) * keySize);
      for(int i=0;i<keySize;i++){
        EEPROM.write(startPos + i, EEPROM.read(lastKeyStart + 1));
        EEPROM.write(lastKeyStart + 1, 0);
      }
    }
    
    EEPROM.write(keyCounter, keyCount - 1); //decrement key counter
  }
}
void openDoor(){
  digitalWrite(pinRelais, HIGH);
  //Timer1.initialize(1000000 * doorSeconds);
  //Timer1.attachInterrupt(doorTimer);
  openUntil = millis()+1500;
  isOpen = true;
}
void checkUID(){
  Serial.println("Check");
  if(searchUID()){
    openDoor();
  }
}

void loop(){
	if(isOpen && millis() > openUntil){
		digitalWrite(pinRelais, LOW);
		isOpen = false;
                //Setup rfid module
                SPI.end();
	        SPI.begin();               // Init SPI bus
                mfrc522.PCD_Init();        // Init MFRC522 card
	}
        uid[0] = 0x00;
        uid[1] = 0x00;
        uid[2] = 0x00;
        uid[3] = 0x00;
        uid[4] = 0x00;
        uid[5] = 0x00;
        uid[6] = 0x00;
        uid[7] = 0x00;
        uid[8] = 0x00;
        uid[9] = 0x00;
        stateAdd = digitalRead(pinAdd);
        stateDel = digitalRead(pinDel);
        stateRouter = digitalRead(pinRouter);
        stateTaster = digitalRead(pinTaster);
        
        if(!stateTaster){
          openDoor();
        }
        
	// Look for new cards
        if ( ! mfrc522.PICC_IsNewCardPresent()) {
            return;
        }
        
        // Select one of the cards
        if ( ! mfrc522.PICC_ReadCardSerial()) {
             return;
        }
        // Now a card is selected. The UID and SAK is in mfrc522.uid.
                
        // Dump UID
        Serial.print("Card UID:");
        for (byte i = 0; i < mfrc522.uid.size; i++) {
             //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
             Serial.print(mfrc522.uid.uidByte[i], HEX);
             uid[i] = mfrc522.uid.uidByte[i];
        } 
        Serial.println();
        
        if(stateAdd){
          saveUID();
        }else if(stateDel){
          deleteUID();
        }else{
          checkUID();
        }
        
        Serial.print("Num Keys:");
        Serial.println(numEntries());
        
        delay(50);
}


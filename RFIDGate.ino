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

const byte
  pinTaster = 7,
  pinRelais = 3,
  pinRouter = 4,
  pinAdd    = 8,
  pinDel    = 9, // ??? TODO! RST PIN!
  pinRST    = 9,
  pinSS     = 10,

  masterUID[10] = {0x14,0x72,0x95,0x5B,0x00,0x00,0x00,0x00,0x00,0x00}, //Programming chip to add new cards

  keySize   = 10,   // num of storage blocks for one uid
  keyStart  = 100;  //storage start position for keys

boolean
  spiBegan,
  stateAdd,
  stateDel,
  stateRouter,
  stateTaster,
  isOpen,
  isAddMode;

MFRC522 mfrc522(pinSS, pinRST);        // Create MFRC522 instance.

byte
  keyCounter  = 0, 	//Storage location for key counter - max 255 keys
  opMode      = 0;  //0=normal,1=add,2=del

int uid[10];
int startPos;

unsigned long
  openUntil,
  addModeUntil;


void setupRFID()
{
  // Reset SPI bus
  SPI.end();
  SPI.begin();

  // Init MFRC522 module
  mfrc522.PCD_Init();
}

void setup(){
	//Debug output
	Serial.begin(115200);
	
  setupRFID();
    
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
   //Timer1.detachInterrupt();
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
    isAddMode = false;
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
void addMode(){
  isAddMode = true;
  addModeUntil = millis()+1500;
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
  if(searchUID() && !isMasterKey()){ //Open door only for normal card, not for master key
    openDoor();
  }
}

void checkMasterKey(){
  //If master key detected, start add mode for some time
  if(isMasterKey()){
    isAddMode = true;
    addModeUntil = millis()+1500;
  }
}

boolean isMasterKey(){
  int matches = 0;
  for(int i=0;i<10;i++){
    if(masterUID[i] == uid[i]) matches++;
  }
  if(matches == 10) return true;
  else return false;
   
}

void doorLoop()
{
  //If openDoor() called and time elapsed, close dor again
  if(isOpen && millis() > openUntil){
    
    digitalWrite(pinRelais, LOW);
    isOpen = false;
    
    setupRFID();            
  }
}

void modeLoop()
{

  //end addMode after time elapsed
  if(isAddMode && millis() > addModeUntil){
    isAddMode = false;
    setupRFID();
  }  
}

void loop()
{
  doorLoop();  // Handle the door state (close it if needed)
  modeLoop();  // Handle the mode timing

  //Reset uid to zero
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
       Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
       Serial.print(mfrc522.uid.uidByte[i], HEX);
       uid[i] = mfrc522.uid.uidByte[i];
  } 
  Serial.println();
  
  checkMasterKey();
  
  Serial.print("Master Key Mode:");
  Serial.println(isAddMode);
  
  if(!stateAdd || isAddMode){
    saveUID();
  }else if(!stateDel){
    deleteUID();
  }else{
    checkUID();
  }
  
  Serial.print("Num Keys:");
  Serial.println(numEntries());
  
  delay(250);
}


//master-keys.h
boolean isMasterKey();
byte
	masterUID[10] = {0x14,0x72,0x95,0x5B,0x00,0x00,0x00,0x00,0x00,0x00}, //Programming chip to add new cards
	statProg	= false; //programming mode on or off

unsigned long timeMasterKey = 0;

void loopMasterKeys()
{
        if(timeMasterKey < millis() && statProg == true){
          statProg = false;
          Serial.println("[masterkeys] master mode off");
        }
	if(isMasterKey()){
          Serial.println("[masterkeys] master mode on");
  	  statProg = true;
          timeMasterKey = millis() + 3000; //3s programming mode
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

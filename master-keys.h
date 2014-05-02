//master-keys.h
const byte
	masterUID[10] = {0x14,0x72,0x95,0x5B,0x00,0x00,0x00,0x00,0x00,0x00}, //Programming chip to add new cards
	stateProg	= false, //programming mode on or off
	progDelayMS = 5000;
	

void loopMasterKeys()
{
	if(isMasterKey){
		stateProg = true;
		Timer1.attachInterrupt(endProgMode, progDelayMS);
	}
}

void endProgMode(){
	stateProg = false;
	Timer1.detachInterrupt();
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

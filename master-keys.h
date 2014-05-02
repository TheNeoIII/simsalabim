//master-keys.h
const byte
	masterUID[10] = {0x14,0x72,0x95,0x5B,0x00,0x00,0x00,0x00,0x00,0x00}; //Programming chip to add new cards

void modeLoop()
{
  // Switch back to normal mode if speical mode timer is reached
  if(addUntil > 0 && millis() > addUntil){
    addUntil = 0;

    // Reset the RFID module after each usage
    resetRFID();
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

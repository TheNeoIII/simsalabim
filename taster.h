//taster.h
byte
  pinTaster = 7,
  statTaster = false;
  
unsigned long timeTaster = 0;
  
void setupTaster(){
	//setup taster input
	pinMode(pinTaster, INPUT_PULLUP);
}

void loopTaster(){
        if(timeTaster < millis() && statTaster == false){
          statTaster = true;
        }
	// Check if the door opener has been pressed
	if (digitalRead(pinTaster) == LOW && statTaster) {
		openDoor();
                statTaster = false;
                timeTaster = millis() + 250; //check taster again in 250ms
                dataString += "[taster] openDoor";
	}
}

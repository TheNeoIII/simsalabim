//taster.h
void setupTaster(){
	//setup taster input
	pinMode(pinTaster, INPUT_PULLUP);
}
void loopTaster(){
	// Check if the door opener has been pressed
	if (digitalRead(pinTaster) == LOW) {
		openDoor();
	}
}

//relais.h
void setupRelais(){
	//relais output
	pinMode(pinRelais, OUTPUT);
	digitalWrite(pinRelais, LOW);
}
void openDoor(){
  Serial.println(F("[door] opening!"));
  digitalWrite(pinRelais, HIGH);
  openUntil = millis()+1500;
}

void loopRelais()
{
  // Close the door if it has been open long enough
  if(openUntil > 0 && millis() > openUntil){

    digitalWrite(pinRelais, LOW);
    openUntil = 0;

    // Reset the RFID module after each usage
    resetRFID();
  }
}

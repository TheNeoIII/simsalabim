//relais.h
byte
  pinRelais = 3,
  statRelais = false;

unsigned long timeRelais = 0;
  
void setupRelais(){
	//relais output
	pinMode(pinRelais, OUTPUT);
}
void closeDoor(){
  digitalWrite(pinRelais, LOW);
  Serial.println(F("[door] closed."));
  statRelais = false;
}
void openDoor(){
  Serial.println("[door] opening!");
  digitalWrite(pinRelais, HIGH);
  timeRelais = millis()+3000;
  statRelais = true;
}

void loopRelais(){
   if(timeRelais < millis() && statRelais == true){
     closeDoor();
   }
}

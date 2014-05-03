/*
  Relais.h - Library for handling the relais control
*/

#ifndef Relais_h
#define Relais_h

#include <Arduino.h>

#define PIN_RELAIS 3

unsigned long timeRelais = 0;

void setupRelais();
void closeDoor();
void openDoor();
void loopRelais();

void setupRelais(){
  pinMode(PIN_RELAIS, OUTPUT);
}

void closeDoor(){
  Serial.println(F("[door] closing."));  
  digitalWrite(PIN_RELAIS, LOW);

  timeRelais = 0;
}

void openDoor(){
  Serial.println(F("[door] opening!"));
  digitalWrite(PIN_RELAIS, HIGH);

  timeRelais = millis()+3000;
}

void loopRelais(){
   if(timeRelais > 0 && timeRelais < millis()){
     closeDoor();
   }
}

#endif

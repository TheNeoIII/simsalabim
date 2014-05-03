/*
  Taster.h - Library for handling the door's button
*/

#ifndef Taster_h
#define Taster_h

#define TASTER_PIN 7

void setupTaster();
void loopTaster();

void setupTaster(){
  pinMode(TASTER_PIN, INPUT_PULLUP);
}

void loopTaster(){
  // Check if the door opener has been pressed
  if (digitalRead(TASTER_PIN) == LOW) {
    openDoor();
  }
}

#endif
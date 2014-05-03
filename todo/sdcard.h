// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 4;

// make a string for assembling the data to log:
String dataString = "";

void setupSDCard()
{
  Serial.print("[sdcard] Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  pinMode(chipSelect, OUTPUT);
  digitalWrite(chipSelect, LOW); //disable sdcard
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("[sdcard] Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("[sdcard] card initialized.");
  digitalWrite(chipSelect, HIGH); //disable sdcard
}

void loopSDCard()
{
  if(dataString != ""){
    digitalWrite(chipSelect, LOW); //enable sdcard
    
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
  
    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("[sdcard] error opening datalog.txt");
    }
    digitalWrite(chipSelect, HIGH); //disable sdcard
    dataString = "";
  }
}

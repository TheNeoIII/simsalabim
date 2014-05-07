#include "Storage.h"

void Storage::setup() {
  Serial.println(F("[storage] setup"));
  SPI.begin();
  
  // Ethernet+SD
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);

  pinMode(PIN_SS, OUTPUT);
  if (!SD.begin(PIN_SS))
  {
    Serial.println(F("[storage] error - couldn't initialize SD module"));
  }
  
  Serial.println(F("[storage] setup done."));
}

bool Storage::openFile(uint8_t mode)
{
  file = SD.open(FILENAME_KEYS, mode);
  if (!file)
  {
    Serial.println(F("[storage] couldn't open key file"));
    return false;
  }
  return true;
}

void Storage::closeFile()
{
  file.close();
}

// Tries to locate the 
long Storage::findKey(byte (&key)[5]) {
  if (!openFile(FILE_READ))
    return KEY_NOT_FOUND;
  
  bool matches = false;
  unsigned long position = 0;
  unsigned long size = file.size();


  Serial.print(F("[storage] current key storage size is "));
  Serial.println(size);

  while (position < size)
  {
    file.seek(position);

    matches = true;
    for (byte i=0; i<5; i++)
    {
      if (file.read() != key[i])
      {
        matches = false;
        break;
      }
    }

    if (matches)
    {
      break;
    }
    position += 28;
  }

  if (!matches)
  {  
    closeFile();
    Serial.println(F("[storage] Key not found."));
    return KEY_NOT_FOUND;
  }

  Serial.print(F("[storage] Found key at position "));
  Serial.println(position/28);

  bool deleted = file.read() == 'N';
  closeFile();

  if (deleted)
  {
    Serial.println(F("[storage] Found key is marked as deleted."));
  }
  return position/28;
}

void Storage::saveKey(byte (&key)[5], byte (&data)[21]) {
  long position = findKey(key);

  if (!openFile(FILE_WRITE))
    return;

  if (position != KEY_NOT_FOUND)
  {
    Serial.println("[storage] jumping to position of key");
    file.seek(position * 28);
  }
  
  Serial.println(F("[storage] saving key..."));

  for (byte i=0; i<5; i++)
    file.write(key[i]);

  file.write("Y");

  for (byte i=0; i<21; i++)
    file.write(data[i]);

  file.write("\n");
  closeFile();
}

void Storage::deleteKey(byte (&key)[5]){
  long position = findKey(key);

  if (!openFile(FILE_WRITE))
    return;

  if (position == KEY_NOT_FOUND)
  {
    Serial.println("[storage] can't delete not exsiting key");
    return;
  }

  Serial.println(F("[storage] setting delete flag of given key"));

  file.seek(position * 28 + 5);
  file.write("N");

  closeFile();
}

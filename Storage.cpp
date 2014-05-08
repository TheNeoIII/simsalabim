#include "Storage.h"

Storage::Storage(byte chipSelect) {
	_pin = chipSelect;
	Serial.println(F("[storage] setup"));
	//SPI.begin();

	// Ethernet+SD
	pinMode(10, OUTPUT); //Requirement for sd lib

	pinMode(_pin, OUTPUT);
	//digitalWrite(_pin, LOW);
	if (!SD.begin(_pin)) {
		Serial.println(F("[storage] error - couldn't initialize SD module"));
	}
	//digitalWrite(_pin, HIGH);

	Serial.println(F("[storage] setup done."));
}

bool Storage::openFile(uint8_t mode) {
	_file = SD.open(FILENAME_KEYS, mode);
	if (!_file) {
		Serial.println(F("[storage] couldn't open key file"));
		return false;
	}
	return true;
}

void Storage::closeFile() {
	_file.close();
}

// Tries to locate the 
long Storage::findKey(byte (&key)[KEY_SIZE]) {
	if (!openFile(FILE_READ))
		return KEY_NOT_FOUND;

	bool matches = false;
	unsigned long position = 0;
	unsigned long size = _file.size();

	Serial.print(F("[storage] current key storage size is "));
	Serial.println(size);

	while (position < size) {
		_file.seek(position);

		matches = true;
		for (byte i = 0; i < KEY_SIZE; i++) {
			if (_file.read() != key[i]) {
				matches = false;
				break;
			}
		}

		if (matches) {
			break;
		}
		position += FRAME_SIZE;
	}

	if (!matches) {
		closeFile();
		Serial.println(F("[storage] Key not found."));
		return KEY_NOT_FOUND;
	}

	Serial.print(F("[storage] Found key at position "));
	Serial.println(position / FRAME_SIZE);

	bool deleted = _file.read() == 'N';
	closeFile();

	if (deleted) {
		Serial.println(F("[storage] Found key is marked as deleted."));
	}
	return position / FRAME_SIZE;
}

void Storage::saveKey(byte (&key)[KEY_SIZE], byte (&data)[21]) {
	long position = findKey(key);

	if (!openFile(FILE_WRITE))
		return;

	if (position != KEY_NOT_FOUND) {
		Serial.println("[storage] jumping to position of key");
		_file.seek(position * FRAME_SIZE);
	}

	Serial.println(F("[storage] saving key..."));

	for (byte i = 0; i < KEY_SIZE; i++)
		_file.write(key[i]);

	_file.write("Y");

	for (byte i = 0; i < 21; i++)
		_file.write(data[i]);

	_file.write("\n");
	closeFile();
}

void Storage::deleteKey(byte (&key)[KEY_SIZE]) {
	long position = findKey(key);

	if (!openFile(FILE_WRITE))
		return;

	if (position == KEY_NOT_FOUND) {
		Serial.println("[storage] can't delete not exsiting key");
		return;
	}

	Serial.println(F("[storage] setting delete flag of given key"));

	_file.seek(position * FRAME_SIZE + KEY_SIZE);
	_file.write("N");

	closeFile();
}

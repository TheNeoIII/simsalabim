simsalabim
==========

Türöffnung Deluxe

Genutzte Libraries
------------------

* Ethernet: https://github.com/ntruchsess/arduino_uip/
* RFID:
  * https://github.com/song940/rfid
  * https://github.com/ljos/MFRC522
* SD: Arduino SD library

Datenstruktur
-------------

Je Schlüssel werden 93 bit benötigt.

|  0  - 39  |
| --------- |
|  RFID ID  |

Danach folgen sieben Blöcke je 24 bit für jede Stunde jedes Wochentags (Montag - Sonntag)

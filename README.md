simsalabim
==========

Türöffnung Deluxe


Datenstruktur
-------------

Je Schlüssel werden 93 bit benötigt.

|  0  - 39  |    40    |    41    |    42    |    43    |
| --------- | -------- | -------- | -------- | -------- |
|  RFID ID  |  Master  | Gruppe A | Gruppe B | Gruppe C |

Danach folgen sieben Blöcke je 7 bit für jeden Wochentag (Montag - Sonntag) 

|   0   |    1   |    2   |    3   |    4   |    5   |    6   |
| ----- | ------ | ------ | ------ | ------ | ------ | ------ |
| 8-10h | 10-12h | 12-14h | 14-16h | 16-18h | 18-20h | 20-22h |

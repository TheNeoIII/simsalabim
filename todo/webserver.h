#include <EtherCard.h>

//webserver.h
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
static byte myip[] = { 192,168,178,49 };
static byte mygw[] = { 192,168,178,1 };

byte Ethernet::buffer[300];
 
void setupWebserver () {
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW); //Enable ethernet module
  
  /*
  Serial.print("[webserver] MAC: ");
  for (byte i = 0; i < 6; ++i) {
    Serial.print(mymac[i], HEX);
    if (i < 5)
      Serial.print(':');
  }
  Serial.println();
  */
  
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "[webserver] Failed to access Ethernet controller");

  Serial.println("[webserver] Setting up IP");
  if (!ether.staticSetup(myip,mygw))
    Serial.println( "[webserver] static IP setting failed");
  
  ether.printIp("[webserver] My IP: ", ether.myip);
  digitalWrite(8, HIGH);//disable ethernet module
}
 
void loopWebserver() {
  digitalWrite(8, LOW);//enable ethernet module
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
 
  if(pos) {   
    
    BufferFiller bfill = ether.tcpOffset();
    
    //Print header
    char* data = (char *) Ethernet::buffer + pos;
    Serial.println(data);
    
    //get name content and print
    
    char getid[20];
    char getdata[100];
    int  posit;
    posit = strcspn( data, "\n" );
    strncpy(getdata, data, posit);
    
    sscanf(getdata, "?name=%99[^&]", getid); //TODO: read id and normalize
        
    Serial.print("[webserver] ");
    Serial.println(getdata);
    
    bfill.emit_p(PSTR("<!DOCTYPE html>"
      "<html>"
        "<head>"
          "<title>SimSalabim</title>"
          "<meta charset=\"utf-8\"/>"
        "</head>"
        "<body>"
          "<h3 style=\"text-align:center;\">Willkommen in der Wechselwelt</h3>"
          "<form method='get' action=''>"
            "ID: <input type='text' name='name' />"
            "<input type='submit' name='submit' value='Open door' />"
        "</body>"
      "</html>"
    ""));
    ether.httpServerReply(bfill.position());
    // Normalize users UID to 10 bytes
    memset(currentUID, 0, 10);
    memcpy(currentUID, getid, sizeof(getid));
    if(findKey() != NOT_FOUND) {
  	openDoor();
         //dataString += "[rfid] key opened door";
    }
    openDoor(); //remove for productive usage!! no uid check here
  }
  
  digitalWrite(8, HIGH);//disable ethernet module
} 

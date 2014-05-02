//webserver.h
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
static byte myip[] = { 192,168,178,49 };

byte Ethernet::buffer[150];
 
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
  if (!ether.staticSetup(myip))
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
    
    /*
    //Print header
    char* data = (char *) Ethernet::buffer + pos;
    Serial.println(data);
    */
    //get name content and print
    /*
    char getname[20];
    char getdata[100];
    int  pos;
    pos = strcspn( data, "\n" );
    strncpy(getdata, data, pos);
    
    sscanf(getdata, "name=%99[^&]", getname);
        
    Serial.print("[webserver] ");
    Serial.println(getname);
    */
    bfill.emit_p(PSTR(""
      "<html>"
        "<head>"
          "<title>SimSalabim</title>"
        "</head>"
        "<body>"
          "<h3 style=\"text-align:center;\">Willkommen in der Wechselwelt</h3>"
          "<form method='get' action=''>"
            "Name: <input type='text' name='name' />"
            "ID: <input type='text' name='id' />"
            "<input type='submit' name='submit' value='Speichern' />"
        "</body>"
      "</html>"
    ""));
    ether.httpServerReply(bfill.position());
    //switch relais for 3sec
    openDoor();
  }
  digitalWrite(8, HIGH);//disable ethernet module
} 

#include <EtherCard.h>

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[700];
static uint32_t timer;

const char website[] PROGMEM = "www.example.com"; //my router's address

// called when the client request is complete
static void my_callback (byte status, word off, word len) {
  Serial.println(">>>");
  Ethernet::buffer[off+len] = 0;
  Serial.print((const char*) Ethernet::buffer + off);
  Serial.println("...");
}

void setup () {
  Serial.begin(57600);
  Serial.println("\n[webClient]");

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
      Serial.println("x{\"Name\":\Hasan"\"}"); /// << JSON message is created here, and the first character of the message is removed
  if (!ether.dhcpSetup())
//    Serial.println("DHCP failed");

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip); 
  ether.printIp("DNS: ", ether.dnsip); 

  if (!ether.dnsLookup(website))
//    Serial.println("DNS failed");

  ether.printIp("SRV: ", ether.hisip);
}

void loop () {
  ether.packetLoop(ether.packetReceive());

  if (millis() > timer) {
    timer = millis() + 5000;
    Serial.println();
    Serial.print("<<< REQ ");
    ether.httpPost(PSTR("/api/test-api"), website, PSTR("Content-Type: application/json"),
       PSTR(""), my_callback); // PSTR("") because the message is created after .begin function is called
    }
}

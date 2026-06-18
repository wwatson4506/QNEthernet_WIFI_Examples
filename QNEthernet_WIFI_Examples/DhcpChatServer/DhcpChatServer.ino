/*
 DHCP Chat  Server

 A simple server that distributes any incoming messages to all
 connected clients.  To use, telnet to your device's IP address and type.
 You can see the client's input in the serial monitor as well.
 Using an Arduino Wiznet Ethernet shield.

 THis version attempts to get an IP address using DHCP

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 21 May 2011
 modified 9 Apr 2012
 by Tom Igoe
 modified 02 Sept 2015
 by Arturo Guadalupi
 Based on ChatServer example by David A. Mellis

 */

// C++ includes
#include <cerrno>
#include <cstring>
#include <ctime>

#include <QNEthernet.h>

using namespace qindesign::network;
constexpr uint32_t kDHCPTimeout = 15000;  // 15 seconds

//#include <SPI.h>
//#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
//byte mac[] = {
//  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02
//};
//IPAddress ip(192, 168, 1, 177);
//IPAddress myDns(192, 168, 1, 1);
//IPAddress gateway(192, 168, 1, 1);
//IPAddress subnet(255, 255, 0, 0);

// telnet defaults to port 23
EthernetServer server(23);
boolean gotAMessage = false; // whether or not you got a message from the client yet

void setup() {
  Serial.begin(115200);
  while (!Serial && (millis() < 4000)) {
    // Wait for Serial
  }
  Serial.printf("Starting...\r\n");

  Serial.printf("Starting Ethernet with DHCP...\r\n");
  if (!Ethernet.begin()) {
    printf("Failed to start Ethernet\r\n");
    return;
  }
  uint8_t mac[6];
  Ethernet.macAddress(mac);  // This is informative; it retrieves, not sets
  Serial.printf("MAC = %02x:%02x:%02x:%02x:%02x:%02x\r\n",
         mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.printf("Waiting for local IP...\r\n");
  if (!Ethernet.waitForLocalIP(kDHCPTimeout)) {
    printf("Failed to get IP address from DHCP\r\n");
    return;
  }

  IPAddress ip = Ethernet.localIP();
  Serial.printf("    Local IP    = %u.%u.%u.%u\r\n", ip[0], ip[1], ip[2], ip[3]);
  ip = Ethernet.subnetMask();
  Serial.printf("    Subnet mask = %u.%u.%u.%u\r\n", ip[0], ip[1], ip[2], ip[3]);
  ip = Ethernet.gatewayIP();
  Serial.printf("    Gateway     = %u.%u.%u.%u\r\n", ip[0], ip[1], ip[2], ip[3]);
  ip = Ethernet.dnsServerIP();
  Serial.printf("    DNS         = %u.%u.%u.%u\r\n", ip[0], ip[1], ip[2], ip[3]);


  // start listening for clients
  server.begin();
}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available();

  // when the client sends the first byte, say hello:
  if (client) {
    if (!gotAMessage) {
      Serial.println("We have a new client");
      client.println("Hello, client!");
      gotAMessage = true;
    }

    // read the bytes incoming from the client:
    char thisChar = client.read();
    // echo the bytes back to the client:
    server.write(thisChar);
    // echo the bytes to the server as well:
    Serial.print(thisChar);
    Ethernet.maintain();
  }
}


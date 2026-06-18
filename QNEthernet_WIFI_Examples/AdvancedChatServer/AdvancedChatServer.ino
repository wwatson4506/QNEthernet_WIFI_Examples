/*
 Advanced Chat Server

 A more advanced server that distributes any incoming messages
 to all connected clients but the client the message comes from.
 To use, telnet to your device's IP address and type.
 You can see the client's input in the serial monitor as well.
 Using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 redesigned to make use of operator== 25 Nov 2013
 by Norbert Truchsess

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
//  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
//};
//IPAddress ip(192, 168, 1, 177);
//IPAddress myDns(192, 168, 1, 1);
//IPAddress gateway(192, 168, 1, 1);
//IPAddress subnet(255, 255, 0, 0);


// telnet defaults to port 23
EthernetServer server(23);

EthernetClient clients[8];

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

  Serial.print("Chat server address:");
  Serial.println(Ethernet.localIP());
}

void loop() {
  // check for any new client connecting, and say hello (before any incoming data)
  EthernetClient newClient = server.accept();
  if (newClient) {
    for (byte i=0; i < 8; i++) {
      if (!clients[i]) {
        Serial.print("We have a new client #");
        Serial.println(i);
        newClient.print("Hello, client number: ");
        newClient.println(i);
        // Once we "accept", the client is no longer tracked by EthernetServer
        // so we must store it into our list of clients
        clients[i] = newClient;
        break;
      }
    }
  }

  // check for incoming data from all clients
  for (byte i=0; i < 8; i++) {
    if (clients[i] && clients[i].available() > 0) {
      // read bytes from a client
      byte buffer[80];
      int count = clients[i].read(buffer, 80);
      // write the bytes to all other connected clients
      for (byte j=0; j < 8; j++) {
        if (j != i && clients[j].connected()) {
          clients[j].write(buffer, count);
        }
      }
    }
  }

  // stop any clients which disconnect
  for (byte i=0; i < 8; i++) {
    if (clients[i] && !clients[i].connected()) {
      Serial.print("disconnect client #");
      Serial.println(i);
      clients[i].stop();
    }
  }
}

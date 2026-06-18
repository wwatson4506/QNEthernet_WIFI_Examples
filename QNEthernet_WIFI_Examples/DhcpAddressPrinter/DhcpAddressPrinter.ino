/*
  DHCP-based IP printer

  This sketch uses the DHCP extensions to the Ethernet library
  to get an IP address via DHCP and print the address obtained.
  using an Arduino Wiznet Ethernet shield.

  Circuit:
   Ethernet shield attached to pins 10, 11, 12, 13

  created 12 April 2011
  modified 9 Apr 2012
  by Tom Igoe
  modified 02 Sept 2015
  by Arturo Guadalupi

 */
// C++ includes
#include <cerrno>
#include <cstring>
#include <ctime>

#include <QNEthernet.h>

using namespace qindesign::network;

constexpr uint32_t kDHCPTimeout = 15000;  // 15 seconds

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield

void setup() {
  Serial.begin(115200);
  while (!Serial && (millis() < 4000)) {
    // Wait for Serial
  }
  printf("Starting...\r\n");

  printf("Starting Ethernet with DHCP...\r\n");
  if (!Ethernet.begin()) {
    printf("Failed to start Ethernet\r\n");
    return;
  }
  uint8_t mac[6];
  Ethernet.macAddress(mac);  // This is informative; it retrieves, not sets
  printf("MAC = %02x:%02x:%02x:%02x:%02x:%02x\r\n",
         mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  printf("Waiting for local IP...\r\n");
  if (!Ethernet.waitForLocalIP(kDHCPTimeout)) {
    printf("Failed to get IP address from DHCP\r\n");
    return;
  }

  IPAddress ip = Ethernet.localIP();
  printf("    Local IP    = %u.%u.%u.%u\r\n", ip[0], ip[1], ip[2], ip[3]);
  ip = Ethernet.subnetMask();
  printf("    Subnet mask = %u.%u.%u.%u\r\n", ip[0], ip[1], ip[2], ip[3]);
  ip = Ethernet.gatewayIP();
  printf("    Gateway     = %u.%u.%u.%u\r\n", ip[0], ip[1], ip[2], ip[3]);
  ip = Ethernet.dnsServerIP();
  printf("    DNS         = %u.%u.%u.%u\r\n", ip[0], ip[1], ip[2], ip[3]);
}

void loop() {
  switch (Ethernet.maintain()) {
    case 1:
      //renewed fail
      Serial.println("Error: renewed fail");
      break;

    case 2:
      //renewed success
      Serial.println("Renewed success");
      //print your local IP address:
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;

    case 3:
      //rebind fail
      Serial.println("Error: rebind fail");
      break;

    case 4:
      //rebind success
      Serial.println("Rebind success");
      //print your local IP address:
      Serial.print("My IP address: ");
      Serial.println(Ethernet.localIP());
      break;

    default:
      //nothing happened
      break;
  }
}


/*
  Link Status
  This sketch prints the ethernet link status. When the
  ethernet cable is connected the link status should go to "ON".
  NOTE: Only WizNet W5200 and W5500 are capable of reporting
  the link status. W5100 will report "Unknown".
  Hardware:
   - Ethernet shield or equivalent board/shield with WizNet 5200/5500
  Written by Cristian Maglie
  This example is public domain.
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

void setup() {
  Serial.begin(115200);
  while (!Serial && (millis() < 4000)) {
    // Wait for Serial
  }
  Serial.printf("Starting...\r\n");
  Serial.printf("Starting Ethernet with DHCP...\r\n");
  if (!Ethernet.begin()) {
    Serial.printf("Failed to start Ethernet\r\n");
    return;
  }
/*

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
*/
}

void loop() {
  auto link = Ethernet.linkStatus();
  Serial.print("Link status: ");
  switch (link) {
    case Unknown:
      Serial.println("Unknown");
      break;
    case LinkON:
      Serial.println("ON");
      break;
    case LinkOFF:
      Serial.println("OFF");
      break;
  }
  delay(1000);
}

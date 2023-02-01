#include <SPI.h>
#include <LoRa.h>

#define ss 4
#define rst 7
#define dio0 5

void setup() {
  Serial.begin(9600);
  while (!Serial);

  delay(5000);

  Serial.println("LoRa Receiver");
  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
      Serial.print((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}

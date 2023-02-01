#include <SPI.h>
#include <LoRa.h>

int counter = 0;

#define ss 4
#define rst 7
#define dio0 5

void setup() {
  Serial.begin(9600);
  while (!Serial);

  delay(5000);

  Serial.println("LoRa Sender");
  LoRa.setPins(ss,rst,dio0);

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);

  // send packet
  LoRa.beginPacket();
  LoRa.print("45.497427, -73.578116; 25 degrees; O2 Level OK");
  LoRa.endPacket();

  counter++;

  delay(5000);
}

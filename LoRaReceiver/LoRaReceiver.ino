#include <LoRa.h>
#include <SPI.h>
int counter = 0;
#define SCK     18 //  SX1278's SCK
#define MISO    19 //  SX1278's MISO
#define MOSI    23 //  SX1278's MOSI
#define SS      5  //  SX1278's CS
#define RST     14 //  SX1278's RESET
#define DI0     2  //  SX1278's IRQ(Interrupt Request)
#define BAND    433E6
void setup() {
  Serial.begin(9600);
  pinMode(32,OUTPUT);
  pinMode(33,OUTPUT);
  digitalWrite(32, HIGH);
  digitalWrite(33, HIGH);
  while (!Serial);

  Serial.println("LoRa Reciever");
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(BAND)) {
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

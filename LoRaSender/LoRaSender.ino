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
  while (!Serial);

  Serial.println("LoRa Sender");
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  digitalWrite(32, !digitalRead(32));
  digitalWrite(33, !digitalRead(33));
  Serial.print("Sending packet: ");
  Serial.println(counter);
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();
  counter++;
  delay(1000);
}

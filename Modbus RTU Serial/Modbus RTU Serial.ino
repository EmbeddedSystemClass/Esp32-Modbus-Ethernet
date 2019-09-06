#include "kitdevesp32.h"
ModbusMaster node;

void setup()
{
  Serial.begin(115200);
  Serial.print("\r\n*****************KIT DEV ESP32*******************");
  Serial_kitdev.begin(256000, SERIAL_8N1); 
  node.begin(2, Serial_kitdev); //ID node 2
  pinMode(27, OUTPUT);
  pinMode(14, OUTPUT);
  digitalWrite(27, HIGH);
  digitalWrite(14, HIGH);
}
void loop()
{ Serial.println("");
  Serial.println("Wait Read");
  uint8_t result;
  result = node.readHoldingRegisters(2, 1);//Doc thanh ghi holding, doc 1 thanh ghi bat dau tu thanh ghi 2
  delay(10);
  if (result == node.ku8MBSuccess) //Read ok
  {
    int16_t signedVal=node.getResponseBuffer(0);
    Serial.print("Value:"); Serial.print(signedVal);         
  }
  else Serial.print("Fail");
  delay(1000);
  digitalWrite(27, !digitalRead(27));
  digitalWrite(14, !digitalRead(14));
}

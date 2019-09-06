#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "kitdevesp32.h"
ModbusMaster node;



char auth[] = "be84e82fe882498d97a4edf43af8d8c4";
char ssid[] = "CAFE NAU";
char pass[] = "nau61tumo";

void setup()
{
  Serial.begin(115200);
  Serial.print("\r\n*****************DEMO ESP32 MODBUS RTU*******************\r\n");
  Serial.print("\r\n*****************THANHNAMLT5@GMAIL.COM*******************\r\n");
  Blynk.begin(auth, ssid, pass);
  Serial_kitdev.begin(9600, SERIAL_8E1);//Baudrate 9600, 8 bit data, parity none, 1 bit stop.
  node.begin(1, Serial_kitdev); //Slave ID node 1
  pinMode(LAN_KITDEV, OUTPUT);
  digitalWrite(LAN_KITDEV, HIGH);//Enable 50MHz for Osc
  delay(1000);
}

void loop()
{
  Serial.println("");
  Serial.println("Wait Read");
  uint8_t result;
  result = node.readHoldingRegisters(0x12, 4);//Doc thanh ghi holding, doc 1 thanh ghi bat dau tu thanh ghi 2
  delay(10);
  
  /*************************************************************READ*******************************************************/
  
  if (result == node.ku8MBSuccess) //Read Modbus RTU ok
  {
    int16_t signedVal=node.getResponseBuffer(0);//ket qua luu tai node.getResponseBuffer(0)
    Serial.print("\r\nValue A:"); 
    Serial.print(signedVal);
    signedVal=node.getResponseBuffer(1);//ket qua luu tai node.getResponseBuffer(0)
    Serial.print("\r\nValue B:"); 
    Serial.print(signedVal);  
    signedVal=node.getResponseBuffer(2);//ket qua luu tai node.getResponseBuffer(0)
    Serial.print("\r\nValue C:"); 
    Serial.print(signedVal);  
    signedVal=node.getResponseBuffer(3);//ket qua luu tai node.getResponseBuffer(0)
    Serial.print("\r\nValue D:"); 
    Serial.print(signedVal);    
    Blynk.virtualWrite(V1,signedVal); 
    Blynk.virtualWrite(V2,signedVal); 
    Blynk.virtualWrite(V3,signedVal); 
    Blynk.virtualWrite(V4,signedVal);      
  }
  else //Read Modbus RTU fail
  {
    Serial.print("Read Fail");
     Blynk.virtualWrite(V1,"Fail");
     Blynk.virtualWrite(V2,"Fail");
     Blynk.virtualWrite(V3,"Fail");
     Blynk.virtualWrite(V4,"Fail");  
  }  
  /***********************************************************RUN BLYNK*****************************************************/
  delay(500);
  Blynk.run(); 
  
}

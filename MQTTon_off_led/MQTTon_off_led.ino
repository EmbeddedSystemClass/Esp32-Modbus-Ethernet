
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "VINH"
#define WLAN_PASS       "f123456789"


/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "m11.cloudmqtt.com"
#define AIO_SERVERPORT  12422
#define AIO_USERNAME    "robbbjvi"
#define AIO_KEY         "ybxRNf9fhlUJ"

/************ Global State (you don't need to change this!) ******************/
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
/****************************** Feeds ***************************************/
Adafruit_MQTT_Publish fb = Adafruit_MQTT_Publish(&mqtt,"fb0");
Adafruit_MQTT_Subscribe rl1 = Adafruit_MQTT_Subscribe(&mqtt,"rl1");
Adafruit_MQTT_Subscribe rl2 = Adafruit_MQTT_Subscribe(&mqtt,"rl2");
void MQTT_connect();
void setup() 
{
  Serial.begin(115200);
  delay(10);
  Serial.println(F("Adafruit MQTT demo"));
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  mqtt.subscribe(&rl1);
  mqtt.subscribe(&rl2);
  pinMode(14, OUTPUT);
  pinMode(27, OUTPUT);  
}
uint32_t x=0,pretime=0; int count_connect_fail=0;
void loop() 
{
  if(WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    count_connect_fail++;
    Serial.println("fail");
    if(count_connect_fail>=20) ESP.restart();
  }
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) 
  {
    if (subscription == &rl1) 
    {
      String chuoi=(char *)rl1.lastread;
      if(chuoi=="1") 
      {digitalWrite(14,HIGH);
      fb.publish("Da bat");}
      else if(chuoi=="0") 
      {digitalWrite(14,LOW);
      fb.publish("Da tat");}
    }
    else if (subscription == &rl2) 
    {
      String chuoi=(char *)rl2.lastread;
      if(chuoi=="1") 
      {digitalWrite(27,HIGH);
      fb.publish("Da bat");}
      else if(chuoi=="0") 
      {digitalWrite(27,LOW);
      fb.publish("Da tat");}
    }
  }
}
void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) 
  {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 5;
  while ((ret = mqtt.connect()) != 0) 
  {
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         ESP.restart();
       }
  }
  Serial.println("MQTT Connected!");
}

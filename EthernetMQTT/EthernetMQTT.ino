#include <ETH.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "m11.cloudmqtt.com"
#define AIO_SERVERPORT  12422
#define AIO_USERNAME    "robbbjvi"
#define AIO_KEY         "ybxRNf9fhlUJ"


static bool eth_connected = false;

void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}

void testClient(const char * host, uint16_t port)
{
  Serial.print("\nconnecting to ");
  Serial.println(host);

  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  client.printf("GET / HTTP/1.1\r\nHost: %s\r\n\r\n", host);
  while (client.connected() && !client.available());
  while (client.available()) {
    Serial.write(client.read());
  }

  Serial.println("closing connection\n");
  client.stop();
}
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
  pinMode(14, OUTPUT);
  digitalWrite(14, HIGH);
  Serial.begin(115200);

  delay(10);
  Serial.println(F("Adafruit MQTT demo"));
  Serial.println(); Serial.println();
  Serial.print("Connecting to ETH");
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  while (!eth_connected) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  mqtt.subscribe(&rl1);
  mqtt.subscribe(&rl2);
  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);  
}
uint32_t x=0,pretime=0; int count_connect_fail=0;
void loop() 
{
  if(!eth_connected) 
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
      {digitalWrite(32,HIGH);
      fb.publish("Da bat");}
      else if(chuoi=="0") 
      {digitalWrite(32,LOW);
      fb.publish("Da tat");}
    }
    else if (subscription == &rl2) 
    {
      String chuoi=(char *)rl2.lastread;
      if(chuoi=="1") 
      {digitalWrite(33,HIGH);
      fb.publish("Da bat");}
      else if(chuoi=="0") 
      {digitalWrite(33,LOW);
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

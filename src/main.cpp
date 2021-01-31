#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "stringToUint_64.h"
#define ARDUINOJSON_USE_LONG_LONG 1
#include <ArduinoJson.h>
#include "esp_secrets.h"

//MQTT Settings
#define MQTT_SERVER "192.168.178.36"
#define MQTT_PORT 1883
#define CLIENT_NAME "ESP8266-LukasRoom"
char *mqttTopic = "lukas-room/ir-esp";
char *mqttResulstTopic = "lukas-room/ir-esp-result";

DynamicJsonDocument doc(4096);

//PIN-Setup
const int lightPin = 2;
const uint16_t irSender = D2;

//Other Stuff
WiFiClient wifiClient;
void callback(char *topic, byte *payload, unsigned int length);
void printWifiStatus();
void reconnect();
void wifiSetup();

PubSubClient client(wifiClient);
IRsend irsend(irSender);

void setup()
{
  Serial.begin(115200);
  delay(100);

  //Wifi-Setup
  wifiSetup();

  //MQTT-Connection to Server
  client.setServer(MQTT_SERVER, MQTT_PORT);

  //IR-Send
  irsend.begin();

  delay(2000);
}

void loop()
{
  if (!client.connected() && WiFi.status() == 3)
  {
    reconnect();
  }

  client.setCallback(callback);
  client.loop();

  delay(10);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  String load;
  Serial.println("Recieved MQTT Topic");
  for (int i = 0; i < length; i++)
  {
    load += (char)payload[i];
  }
  Serial.print("Payload: ");
  Serial.println(load);
  Serial.println();

  //Payload to JSON Object
  deserializeJson(doc, load);

  String protocol = doc["protocol"];
  int bits = doc["bits"];
  String data = doc["data"];

  //Convert IR-Payload -> uint64 to user ir library
  uint64_t ir_data_converted;
  ir_data_converted = stringToUint_64(data);

  //Send Command
  if (protocol == "NEC")
  {
    irsend.sendNEC(ir_data_converted, bits);
    client.publish(mqttResulstTopic, "IR-DATA Send");
  }
  else if (protocol != "NEC")
  {
    String error_msg = "Protocol ";
    error_msg.concat(protocol);
    error_msg.concat(" not supported yet.");

    client.publish(mqttResulstTopic, error_msg.c_str());
  }
}

void reconnect()
{
  //reconnect to WiFi if connection lost
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Connecting to WiFi: ");
    Serial.println(WIFI_SSID);

    //retry ever 0.5s
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
  printWifiStatus();
  }
  //wifi has to connected to succesfully connect to mqtt server
  if (WiFi.status() == WL_CONNECTED)
  {
    while (!client.connected())
    {
      Serial.println("Attempting MQTT connection...");
      if (client.connect(CLIENT_NAME, MQTT_USER, MQTT_PASSWORD))
      {
        Serial.println("MQTT Connected with Clientname: " CLIENT_NAME);
        client.subscribe(mqttTopic);
      }
      else
      {
        Serial.println("\tFailed.");
        abort();
      }
    }
  }
}

void wifiSetup()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to Wifi: " WIFI_SSID);
  delay(250);
  printWifiStatus();
}

void printWifiStatus()
{
  Serial.println("");
  Serial.print("ESP8266 IP-Adress: ");
  Serial.print(WiFi.localIP());
  Serial.println("");
}
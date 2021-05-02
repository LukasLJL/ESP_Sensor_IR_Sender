# ESP8266 Temperatur Sensor with IR Sender
Measure the Temperature and Humidity of your Room and send some IR-Commands to control Infrared capable Devices in your Room via MQTT.

# Requirements
- ESP8266
- IR Sender
- BME280

# Instructions
To compile the Code you have to add a secrets header file into the src directory. This file is for your WiFi and MQTT Credentials. The file name should be ``esp_secrets.h`` <br> 
Here is an example:
````c
//Wifi-Settings
#define WIFI_SSID "MyWiFiSSID"
#define WIFI_PASSWORD "MyWiFiPassword"

//MQTT-Settings
#define MQTT_USER "MyMQTTUser"
#define MQTT_PASSWORD "MyMQTTPassword"
````

If you want to change the IP-Address of the MQTT-Server or the MQTT-Topic you have to edit the main.cpp file.



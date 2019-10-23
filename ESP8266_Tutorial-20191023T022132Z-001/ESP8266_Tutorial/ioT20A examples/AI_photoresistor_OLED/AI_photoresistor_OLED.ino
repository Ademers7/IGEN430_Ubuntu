/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "SSD1306.h" 
#include "topmost.h"

const uint8_t scl = D1;
const uint8_t sda = D2;
SSD1306  display(0x3c, D2, D1);

// Update these with values suitable for your network.

//const char* ssid = "topmostasus";
//const char* mqtt_server = "broker.mqtt-dashboard.com";
const char* ssid = "topinst";
const char* password = "80397876";
const char* mqtt_server = "192.168.0.104";

int sensorPin = A0;
int sensorValue = 0;



WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
 }

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}



void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Attempt to connect
    if (client.connect("ESP8266Client", "topmost", "3310110")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
 //     client.publish("outTopic", "hello world");
      // ... and resubscribe
 //     client.subscribe("inTopic");
 //   } else {
  //    Serial.print("failed, rc=");
 //     Serial.print(client.state());
 //     Serial.println(" try again in 5 seconds");
 //     // Wait 5 seconds before retrying
 //     delay(5000);
    }
  }
}


void loop() {

  if (!client.connected()) {
    display.clear();
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "TOPMOST Instruments");
    display.drawXbm(0, 18, topmost_width, topmost_height, topmost_bits);
    // display.drawString(0, 54, "www.topinst.com.tw");
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(44, 24, "Please Wait!.");
    display.drawString(44, 34, "CONNECTING...");
    display.drawString(44, 44, "Mosquitto");
    display.display();
    reconnect();
      }
   client.loop();
   
   int sensorValue = analogRead(A0);
   float voltage = sensorValue * (3.3 / 1023.0);
   char buf[10];
   snprintf(buf, sizeof(buf), "%.2f", voltage);
   client.publish("topinst", buf);
   display.clear();
   display.setTextAlignment(TEXT_ALIGN_LEFT);
   display.setFont(ArialMT_Plain_10);
   display.drawString(0, 0, "TOPMOST Instruments");
   display.setFont(ArialMT_Plain_16);
   display.drawString(0, 14, "Light Intensity (V)");
   display.setFont(ArialMT_Plain_24);
   display.drawString(0, 34, String(buf) );
   display.display();
   Serial.println(buf);
   delay(100);  
  
  }

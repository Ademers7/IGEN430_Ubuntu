#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//Ubuntu 
//const char* ssid = "A4P-Ubuntu";
//const char* password =  "AimForPass";
//const char* broker_ip = "10.42.0.1";
//Rpi
const char* ssid = "A4PAP";
const char* password= "AimForPass";
const char* broker_ip = "192.168.0.10";
String clientId = "A4P_NET1";
const int mqttPort = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

//Effect: check if the parameter payload is "ENABLE"
bool strIsEnable(byte* payload, unsigned int length){
  if(length != 6){
    return false;
  }
  else{
    char* enable = "ENABLE";
    bool equal = true;
    
    for(int i = 0; i < length; i++){
      if(enable[i] != payload[i]){
        equal = false;
      }
    }
    
    return equal;
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println(topic);
  Serial.print("Message:");
  if(strIsEnable(payload, length)){
    //the command is "ENABLE"
    //TODO: Aidan put your code here  
    Serial.println("enable!!");
  }
  else{
    //the command is not "ENABLE"  
  }
}

void connectToWifi(){
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to the WiFi network");
}

void connectToBroker() {
  client.setServer(broker_ip, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.print("Connecting to Mqtt Broker...");
    // Attempt to connect
    //String clientId = "A4P_NET1";
    //clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected")gi;
      String topic = "topic/" + clientId;
      client.subscribe(topic.c_str());

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  connectToWifi();
  connectToBroker();
}
 

 
void loop() {
  if(!espClient.connected()){
     connectToWifi();
  }
  if(!client.connected()){
     connectToBroker(); 
  }
  client.loop();
}

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "topmost.h"
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`
SSD1306  display(0x3c, D2, D1);

const char* ssid = "topinst";
const char* password = "80397876";
const char* mqtt_server = "192.168.0.104";


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

 
// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;

// Select SDA and SCL pins for I2C communication 
const uint8_t scl = D1;
const uint8_t sda = D2;

// sensitivity scale factor respective to full scale setting provided in datasheet 
const uint16_t AccelScaleFactor = 16384;
const uint16_t GyroScaleFactor = 131;

// MPU6050 few configuration register addresses
const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;

int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  
  Wire.begin(sda, scl);
  MPU6050_Init();
  
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
 }

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
//  Serial.println();
 // Serial.print("Connecting to ");
 // Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
 //   Serial.print(".");
  }
//  Serial.println("");
//  Serial.println("WiFi connected");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
  //  Serial.print("Attempting MQTT connection...");
    
    // Attempt to connect
    if (client.connect("ESP8266Client", "topmost", "3310110")) {
  //    Serial.println("connected");
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

  double Ax, Ay, Az, T, Gx, Gy, Gz;
  display.clear();
  Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
  
  //divide each with their sensitivity scale factor
  Ax = (double)AccelX/AccelScaleFactor;
  Ay = (double)AccelY/AccelScaleFactor;
  Az = (double)AccelZ/AccelScaleFactor;
  T = (double)Temperature/340+36.53; //temperature formula
  Gx = (double)GyroX/GyroScaleFactor;
  Gy = (double)GyroY/GyroScaleFactor;
  Gz = (double)GyroZ/GyroScaleFactor;

  // Serial.print("Ax: "); Serial.print(Ax);
  // Serial.print(" Ay: "); Serial.print(Ay);
  // Serial.print(" Az: "); Serial.print(Az);
  // Serial.print(" T: "); Serial.print(T);
  // Serial.print(" Gx: "); Serial.print(Gx);
  // Serial.print(" Gy: "); Serial.print(Gy);
 // Serial.print(" Gz: "); Serial.println(Gz);
 
  display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    display.drawString(0, 0, "TOPMOST Instruments");
    display.setFont(ArialMT_Plain_16);
    display.drawString(0, 14, "Temp. = " +  String (T) + "  C");
  // display.setFont(ArialMT_Plain_24);
     display.setFont(ArialMT_Plain_16);
    // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (3.3 / 1023.0);
    display.drawString(0, 34, "Light   = " + String(voltage) + "     V");
 display.display();

 char buf[50];
   snprintf(buf, sizeof(buf), "%.2f", voltage);
   sprintf(buf, "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f", voltage,Ax,Ay,Az,T,Gx,Gy,Gz);
   client.publish("topinst", buf);
  Serial.println(buf);
 
  delay(20);
}

void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}

// read all 14 register
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (uint8_t)14);
  AccelX = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelY = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelZ = (((int16_t)Wire.read()<<8) | Wire.read());
  Temperature = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroX = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroY = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroZ = (((int16_t)Wire.read()<<8) | Wire.read());
}

//configure MPU6050
void MPU6050_Init(){
  delay(150);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
}


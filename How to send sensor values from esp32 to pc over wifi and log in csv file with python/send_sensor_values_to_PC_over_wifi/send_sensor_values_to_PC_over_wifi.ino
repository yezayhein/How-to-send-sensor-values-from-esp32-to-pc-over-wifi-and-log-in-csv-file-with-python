#include <WiFi.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <Adafruit_SGP30.h>
#include <DHT.h>

// WiFi network name and password:
const char * networkName = "WiFi_Name";
const char * networkPswd = "123456789";

//IP address to send UDP data to PC:
// either use the ip address of the server or 
// a network broadcast address
const char * udpAddress = "192.168.74.98";
const int udpPort = 3333;

//Are we currently connected?
boolean connected = false;

//The udp library class
WiFiUDP udp;

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
Adafruit_SGP30 sgp;
//Adafruit_SGP40 sgp;
const int mq2Pin = 33;
const int mq7Pin = 32;
const int mq9Pin = 35;
const int mq135Pin = 34;
int buzzerPin = 25; //Define buzzerPin

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  if (! sgp.begin()){
    Serial.println("Sensor not found :(");
    while (1);
  }
  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);
  dht.begin();

  //Connect to the WiFi network
  connectToWiFi(networkName, networkPswd);
  delay(1000); 

}

void loop() {
  sgp.IAQmeasure();
  int mq2 = analogRead(mq2Pin);
  int mq7 = analogRead(mq7Pin);
  int mq9 = analogRead(mq9Pin);
  int mq135 = analogRead(mq135Pin);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  int co2 = sgp.eCO2;
  int tvoc = sgp.TVOC;

  // print Sensor values and predicted results of meta
  Serial.print(t);
  Serial.print(",");
  Serial.print(h);
  Serial.print(",");
  Serial.print(co2);
  Serial.print(",");
  Serial.print(tvoc);
  Serial.print(",");
  Serial.print(mq2);
  Serial.print(",");
  Serial.print(mq7);
  Serial.print(",");
  Serial.print(mq9);
  Serial.print(",");
  Serial.println(mq135);

  //only send data when connected
  if(connected){
    //Send a packet
    udp.beginPacket(udpAddress,udpPort);
    //udp.printf("Seconds since boot: %lu", millis()/1000);
    //udp.printf("61066");
    
    // Convert sensor data to strings and send them
    udp.print(t);     // Temperature
    udp.print(",");
    udp.print(h);     // Humidity
    udp.print(",");
    udp.print(co2); // CO2
    udp.print(",");
    udp.print(tvoc);  //TVOC
    udp.print(",");
    udp.print(mq2);  //MQ2
    udp.print(",");
    udp.print(mq7);  //MQ7
    udp.print(",");
    udp.print(mq9);  //MQ9
    udp.print(",");
    udp.print(mq135); //MQ135
    udp.endPacket();
  }
  delay(3000);
}
//wifi function
void connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  //register event handler
  WiFi.onEvent(WiFiEvent);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
}
//wifi event handler
void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case ARDUINO_EVENT_WIFI_STA_GOT_IP:
          //When connected set 
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          udp.begin(WiFi.localIP(),udpPort);
          connected = true;
          break;
      case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          connected = false;
          break;
      default: break;
    }
}


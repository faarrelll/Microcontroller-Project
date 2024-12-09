#include <WiFi.h>
#include <PubSubClient.h>
#include "DFRobot_BloodOxygen_S.h"
#define I2C_ADDRESS    0x57

DFRobot_BloodOxygen_S_I2C MAX30102(&Wire ,I2C_ADDRESS);

//const char* ssid = "OCEAN";
//const char* password = "cerdas2023";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "broker.mqtt-dashboard.com";

//WiFiClient espClient;
//PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

float temperature = 0;
int spo2 = 0;
int hrate = 0;

void setup() {
  
  Serial.begin(115200);
  while (false == MAX30102.begin())
  {
    Serial.println("init fail!");
    delay(1000);
  }
  Serial.println("init success!");
  Serial.println("start measuring...");
  MAX30102.sensorStartCollect();
//  setup_wifi();
//  client.setServer(mqtt_server, 1883);
}

//void setup_wifi() {
//  delay(10);
//  // We start by connecting to a WiFi network
//  Serial.println();
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
//
//  WiFi.begin(ssid, password);
//
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//
//  Serial.println("");
//  Serial.println("WiFi connected");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
//}

//
//void reconnect() {
//  // Loop until we're reconnected
//  while (!client.connected()) {
//    Serial.print("Attempting MQTT connection...");
//    // Attempt to connect
//    if (client.connect("ESP32")) {
//      Serial.println("connected");
//      // Subscribe
//      client.subscribe("device/temp");
//    } else {
//      Serial.print("failed, rc=");
//      Serial.print(client.state());
//      Serial.println(" try again in 5 seconds");
//      // Wait 5 seconds before retrying
//      delay(5000);
//    }
//  }
//}
void loop() {
//  if (!client.connected()) {
//    reconnect();
//    // client.publish("device/temp", "ESP32 Online");
//  }
//  client.loop();

  long now = millis();
  if (now - lastMsg > 1000) {
    lastMsg = now;

    MAX30102.getHeartbeatSPO2();
    //temp
    temperature = MAX30102.getTemperature_C();
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.println(tempString);

    //spo2
    spo2 = MAX30102._sHeartbeatSPO2.SPO2;
    char spo2String[8];
    dtostrf(spo2, 1, 2, spo2String);
    Serial.print("SP02 : ");
    Serial.println(spo2String);

    //heart rate
    hrate = MAX30102._sHeartbeatSPO2.Heartbeat;
    char hrateString[8];
    dtostrf(hrate, 1, 2, hrateString);
    Serial.print("heart rate : ");
    Serial.println(hrateString);
    
//    if(spo2 != -1 && hrate != -1){
//      client.publish("device/temp", tempString);
//      client.publish("device/spo2", spo2String);
//      client.publish("device/hrate", hrateString);
//
//    }
    

    
  }
}

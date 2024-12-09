
#include <WiFiManager.h>
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>
#include "DHT.h"


#define DHTPIN1 33    // Digital pin connected to the DHT sensor


#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321

DHT dht1(DHTPIN1, DHTTYPE);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Json Variable to Hold Sensor Readings
JSONVar readings;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 1000;
unsigned long startTime = 0; // Waktu mulai stopwatch dalam milidetik




// Get Sensor Readings and return JSON object
String getSensorReadings(){
  readings["temperature"] = String(dht1.readTemperature());
  readings["humidity"] =  String(dht1.readHumidity());
  readings["jenisSepatu"] =  "Casual";
  readings["waktu"] =  getStopwatchTime();
  readings["status"] =  "Dalam Proses Pengeringan";
// readings["waktu"] =  String(currentHour) + " jam";
// readings["waktu"] =  String(currentHour, DEC) + ":" + String(currentMinute, DEC) + ":" + String(currentSecond, DEC);
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

// Initialize WiFi
void initWiFi() {
//  WiFi.mode(WIFI_STA);
    WiFiManager wm;
    bool res;
    res = wm.autoConnect("Pengering Sepatu","12345678"); // password protected ap
    if(!res) {
        Serial.println("Failed to connect");
         ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
    }
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);
  dht1.begin();
  initWiFi();
  initSPIFFS();
  Serial.println(WiFi.localIP());

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = getSensorReadings();
    request->send(200, "application/json", json);
    json = String();
  });

  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  // Start server
  server.begin();
  startTime = millis();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    // Send Events to the client with the Sensor Readings Every 10 seconds
    events.send("ping",NULL,millis());
    events.send(getSensorReadings().c_str(),"new_readings" ,millis());
    lastTime = millis();

  }

}

String getStopwatchTime() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;

  // Hitung jam, menit, detik dari waktu yang berlalu
  unsigned long hours = elapsedTime / 3600000;
  unsigned long minutes = (elapsedTime % 3600000) / 60000;
  unsigned long seconds = ((elapsedTime % 3600000) % 60000) / 1000;

  // Buat format string untuk stopwatch
  String stopwatchTime = String(hours) + ":" + String(minutes) + ":" + String(seconds);
  
  return stopwatchTime;
}

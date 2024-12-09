#include <WiFi.h>
#include <Wire.h>
#include "RTClib.h"
#include <WebServer.h>

// Set your SSID and Password
const char* ssid = "OCEAN";
const char* password = "cerdas2023";

// Static IP address configuration
IPAddress local_IP(192, 168, 1, 184);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // Optional
IPAddress secondaryDNS(8, 8, 4, 4); // Optional

// Create an instance of the server
WebServer server(80);

// Create an instance of the RTC
RTC_DS3231 rtc;

const int relay1 = 16;
const int relay2 = 17;

void setup() {
  Serial.begin(115200);

  // Configure static IP
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);

  server.on("/", handleRoot);
  server.on("/relay1_on", handleRelay1On);
  server.on("/relay1_off", handleRelay1Off);
  server.on("/relay2_on", handleRelay2On);
  server.on("/relay2_off", handleRelay2Off);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  DateTime now = rtc.now();
  String currentTime = String(now.year()) + "/" + String(now.month()) + "/" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());

  String html = "<!DOCTYPE html><html>";
  html += "<head><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>ESP32 Web Server</title></head>";
  html += "<body><h1>ESP32 Web Server with RTC</h1>";
  html += "<p>Current time: " + currentTime + "</p>";
  html += "<button onclick=\"location.href='/relay1_on'\" type='button'>Relay 1 ON</button>";
  html += "<button onclick=\"location.href='/relay1_off'\" type='button'>Relay 1 OFF</button>";
  html += "<button onclick=\"location.href='/relay2_on'\" type='button'>Relay 2 ON</button>";
  html += "<button onclick=\"location.href='/relay2_off'\" type='button'>Relay 2 OFF</button>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleRelay1On() {
  digitalWrite(relay1, HIGH);
  server.send(200, "text/html", "Relay 1 is ON <br><br> <a href='/'>Back</a>");
}

void handleRelay1Off() {
  digitalWrite(relay1, LOW);
  server.send(200, "text/html", "Relay 1 is OFF <br><br> <a href='/'>Back</a>");
}

void handleRelay2On() {
  digitalWrite(relay2, HIGH);
  server.send(200, "text/html", "Relay 2 is ON <br><br> <a href='/'>Back</a>");
}

void handleRelay2Off() {
  digitalWrite(relay2, LOW);
  server.send(200, "text/html", "Relay 2 is OFF <br><br> <a href='/'>Back</a>");
}

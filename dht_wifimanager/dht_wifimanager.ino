#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <WiFiManager.h>

#define DHTPIN1 33    // Digital pin connected to the DHT sensor


#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321

DHT dht1(DHTPIN1, DHTTYPE);


void setup() {
  Serial.begin(9600);
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
  Serial.println(F("DHT11 test!"));

  dht1.begin();

}

void loop() {

  float h = dht1.readHumidity();

  float t = dht1.readTemperature();


  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT1 sensor!"));
    return;
  }
  Serial.println("Sensor 1");
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
}

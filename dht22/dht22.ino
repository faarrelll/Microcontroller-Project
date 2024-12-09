#include "DHT.h"
#include <LiquidCrystal_I2C.h>

#define DHTPIN1 17     // Digital pin connected to the DHT sensor
#define DHTPIN2 32

//#define DHTPIN3 3
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)


DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
//DHT dht3(DHTPIN3, DHTTYPE);

void setup() {
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));

  dht1.begin();
  dht2.begin();
//  dht3.begin();
}


void loop() {
  // Wait a few seconds between measurements.

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht1.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht1.readTemperature();
  float h2 = dht2.readHumidity();
  // Read temperature as Celsius (the default)
  float t2 = dht2.readTemperature();
//  float h3 = dht3.readHumidity();
//  // Read temperature as Celsius (the default)
//  float t3 = dht3.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT1 sensor!"));
    return;
  }
  if (isnan(h2) || isnan(t2) ) {
    Serial.println(F("Failed to read from DHT1 sensor!"));
    return;
  }
//  if (isnan(h3) || isnan(t3) ) {
//    Serial.println(F("Failed to read from DHT1 sensor!"));
//    return;
//  }


  Serial.println("Sensor 1");
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.println("Sensor 2");
  Serial.print(F("Humidity: "));
  Serial.print(h2);
  Serial.print(F("%  Temperature: "));
  Serial.print(t2);
//  Serial.print(F("°C "));
//  Serial.println("Sensor 3");
//  Serial.print(F("Humidity: "));
//  Serial.print(h3);
//  Serial.print(F("%  Temperature: "));
//  Serial.print(t3);
//  Serial.print(F("°C "));
//  delay(2000);

}

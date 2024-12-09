#include <WiFi.h>
#include <ThingSpeak.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_Sensor.h>

const char* ssid = "Hai";

const char* password = "hai12345";

WiFiClient client;

unsigned long myChannelNumber = 1;
const char* myWriteAPIKey = "O5MTXKFDDXQ57OXI";

//timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

Adafruit_AHTX0 aht;

float suhu, kelembaban;

void setup() {
  Serial.begin(115200);
  aht.begin();
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);

}

void loop() {
  // put your main code here, to run repeatedly:
  if((millis() - lastTime) > timerDelay){

    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid,password);
        delay(5000);
      }
      Serial.println("\nConnected.");
    }
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp); // populate temp and humidity objects with fresh data
    suhu = terelmp.temperature;
    kelembaban = humidity.ative_humidity;

    ThingSpeak.setField(1, kelembaban);
    ThingSpeak.setField(2, suhu);
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if(x == 200){
    Serial.println("Channel update successful.");
    } else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
    lastTime = millis();
  }
}

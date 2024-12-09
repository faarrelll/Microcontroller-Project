#include <WiFi.h>
#include <ThingSpeak.h>
#include <Wire.h>
#include <BH1750.h>



BH1750 luxMeter;
WiFiClient client;

const char* ssid = "Hai";

const char* password = "hai12345";



unsigned long channel = 1;
const char* apikey = "IA996PK4Z2DC5JIF";

int trig = 5;
int echo = 18;

long duration;
float cm;
float inch;




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  Wire.begin();
  luxMeter.begin();
  connectWifi();
  ThingSpeak.begin(client);
}

void loop() {
  // put your main code here, to run repeatedly:
  

  if(WiFi.status() == WL_CONNECTED){

    float lux = luxMeter.readLightLevel();
  digitalWrite(trig, LOW);
  delayMicroseconds(2); 
  digitalWrite(trig, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  cm = duration*0.034/2;
  inch = cm * 0.393701;

  Serial.print("durasi = ");
  Serial.println(duration);
  Serial.print("jarak cm = ");
  Serial.println(cm);
  Serial.print("jarak inch = ");
  Serial.println(inch);
  Serial.print("lux = ");
  Serial.println(lux);
    ThingSpeak.setField(1,duration);
    ThingSpeak.setField(2,cm);
    ThingSpeak.setField(3,inch);
    ThingSpeak.setField(4,lux);
    int x = ThingSpeak.writeFields(channel,apikey);

    if(x == 200){
      Serial.println("succes upddate!");
    } else{
      Serial.println("Failed Update!");
    }

    delay(20000);
  } else{
    connectWifi();
  }
}

void connectWifi(){
  Serial.print("Connecting to...");
  Serial.println(ssid);
  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Wifi Connected");
 
}



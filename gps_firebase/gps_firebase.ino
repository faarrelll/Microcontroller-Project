
/**
 * Created by K. Suwatchai (Mobizt)
 *
 * Email: k_suwatchai@hotmail.com
 *
 * Github: https://github.com/mobizt/Firebase-ESP-Client
 *
 * Copyright (c) 2022 mobizt
 *
 */

/** This example will show how to authenticate using
 * the legacy token or database secret with the new APIs (using config and auth data).
 */


#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include <TinyGPS++.h>
#include <addons/RTDBHelper.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial SerialGPS(4, 5);
//TX = D2, RX = D1 

/* 1. Define the WiFi credentials */
#define WIFI_SSID "PLAY MEDIA"
#define WIFI_PASSWORD "11223344"


/* 2. If work with RTDB, define the RTDB URL and database secret */
#define DATABASE_URL "tjalan-e1cf2-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "IFCwnrdumsUHTsfPbOGbK2p3VETbKsECjc5a7VxQ"
#define triggerPin  D5
#define echoPin     D6
#define buz         D8
#define water A0
#define modecuy     D7
#define penjemputan D3



FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

double Latitude , Longitude;

int lati,c;
int longi;
String DateString , TimeString, LatitudeString , LongitudeString;
unsigned long previousMillis = 0;   
bool modee;   
int air;
long jarak, duration;
int count = 0;
int year , month , date, hour , minute , second;


void setup()
{

    Serial.begin(9600);
    SerialGPS.begin(9600);
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(buz,OUTPUT);
    pinMode(water,INPUT);
    pinMode(modecuy,INPUT_PULLUP);
    pinMode(penjemputan,INPUT_PULLUP);
  
}

void loop()
{
//    Firebase.RTDB.getBool(&fbdo, FPSTR("/lokasi/mode")); 
//    modee = fbdo.to<bool>();
    air = analogRead(water);
    duration, jarak;
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2); 
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10); 
    digitalWrite(triggerPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    jarak = (duration/2) / 29.1;
    int modevar = digitalRead(modecuy);
    if(modevar == 0){
      while (WiFi.status() != WL_CONNECTED)
      {
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        Serial.print("Connecting to Wi-Fi");
        while (WiFi.status() != WL_CONNECTED)
        {
        Serial.print(".");
        delay(300);
        }
        Serial.println();
        Serial.print("Connected with IP: ");
        Serial.println(WiFi.localIP());
        Serial.println();

        Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
        config.database_url = DATABASE_URL;
        config.signer.tokens.legacy_token = DATABASE_SECRET;

        Firebase.reconnectWiFi(true);
        Firebase.begin(&config, &auth);
        digitalWrite(buz,HIGH);
        delay(500);
        digitalWrite(buz,LOW);
        delay(500);
        digitalWrite(buz,HIGH);
        delay(500);
        digitalWrite(buz,LOW);
        delay(500);
        digitalWrite(buz,HIGH);
        delay(500);
        digitalWrite(buz,LOW);
        delay(500);
        
      }
      int nilaibutton = digitalRead(penjemputan);
      if(nilaibutton == 0){
        if(count == 0){
        Firebase.RTDB.setString(&fbdo,"/lokasi/penjemputan","-");
        Serial.println("kuy");
        count = 1;
        }
      } 
      else{
        if(count == 1){
        Firebase.RTDB.setString(&fbdo,"/lokasi/penjemputan","Jemput");
        Serial.println("hop");
        count = 0;
      }
      }
      if(jarak < 50) {
         digitalWrite(buz, HIGH);
      }
      else if(air>350) {
        digitalWrite(buz,HIGH);
        } else {
          digitalWrite(buz,LOW);
          }
       lokasi();
        } 

        
     else{
       if(jarak < 50){
         digitalWrite(buz, HIGH);
         delay(10);
         digitalWrite(buz, LOW);
         delay(50);
//         Serial.println("awas objek terlalu dekat");
      }
      else if(air>350) {
        digitalWrite(buz,HIGH);
        }  
      else {
          digitalWrite(buz,LOW);
          }
        }
     
      
//   if (millis() - previousMillis >= 5){
//    Serial.println("interupt");
//   pembacaan();
//    previousMillis = millis();
//  }
//    Serial.flush();
        
        
    
}

void lokasi(){
  while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
    {
      if (gps.location.isValid())
      {
        Latitude = gps.location.lat();
        LatitudeString = String(Latitude , 6);
        Longitude = gps.location.lng();
        LongitudeString = String(Longitude , 6);
        Serial.print("lat = ");
        Serial.println(Latitude);
        Serial.print("long = ");
        Serial.println(Longitude);
        Firebase.RTDB.setString(&fbdo, "/lokasi/long", LongitudeString);
        Firebase.RTDB.setString(&fbdo, "/lokasi/lat", LatitudeString);
        if (gps.date.isValid())
        {
          DateString = "";
          date = gps.date.day();
          month = gps.date.month();
          year = gps.date.year();

          if (date < 10)
          DateString = '0';
          DateString += String(date);

          DateString += ".";

          if (month < 10)
          DateString += '0';
          DateString += String(month);
          DateString += ".";

          if (year < 10)
          DateString += '0';
          DateString += String(year);
          Serial.println(DateString);
          Firebase.RTDB.setString(&fbdo, "/lokasi/tanggal", DateString);
      }
      if (gps.time.isValid())
      {
          TimeString = "";
          hour = gps.time.hour()+ 7; //adjust UTC
          minute = gps.time.minute();
          
    
          if (hour < 10)
          TimeString = '0';
          TimeString += String(hour);
          TimeString += ".";

          if (minute < 10)
          TimeString += '0';
          TimeString += String(minute);
         

          
          Serial.print(TimeString);
          Firebase.RTDB.setString(&fbdo, "/lokasi/waktu", TimeString);
      }        
      }
    }
  
  }

void pembacaan(){

  air = analogRead(water);
  duration, jarak;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  jarak = (duration/2) / 29.1;
    
  }
  

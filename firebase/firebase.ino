
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

#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <ArduinoJson.h>


// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>


/* 1. Define the WiFi credentials */
#define pwma D1  // 16 corresponds to GPIO16
#define pwmb D2 // 17 corresponds to GPIO17
#define pwmc D3  // 5 corresponds to GPIO5
#define pwmd D4
#define lamp_d D5
#define lamp_b D7
#define riting_kn D8
#define riting_kr D0
//#define buzzer 1

#define WIFI_SSID "TES"
#define WIFI_PASSWORD "11223344"

/* 2. If work with RTDB, define the RTDB URL and database secret */
#define DATABASE_URL "testkontrol-bc934-default-rtdb.asia-southeast1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "tDr3iFZw0HNaVNIhTtk1bqNxoO7OjiGzb96mjVtE"

/* 3. Define the Firebase Data object */
FirebaseData fbdo;

/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;
bool maju,kiri,kanan,mundur,klakson;
bool ld,lb,rkn,rkr;
int pwm;
void setup()
{

    Serial.begin(115200);
//    pinMode(buzzer,OUTPUT);
    pinMode(lamp_d,OUTPUT);
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

    /* Assign the certificate file (optional) */
    // config.cert.file = "/cert.cer";
    // config.cert.file_storage = StorageType::FLASH;

    /* Assign the database URL and database secret(required) */
    config.database_url = DATABASE_URL;
    config.signer.tokens.legacy_token = DATABASE_SECRET;

    Firebase.reconnectWiFi(true);

    /* Initialize the library with the Firebase authen and config */
    Firebase.begin(&config, &auth);
    FirebaseJson json;       // or constructor with contents e.g. FirebaseJson json("{\"a\":true}");
    FirebaseJsonArray arr;   // or constructor with contents e.g. FirebaseJsonArray arr("[1,2,true,\"test\"]");
    FirebaseJsonData result;

    // Or use legacy authenticate method
    // Firebase.begin(DATABASE_URL, DATABASE_SECRET);
}

void loop()
{
    
        //Serial.printf("Set int... %s\n", Firebase.RTDB.setInt(&fbdo, "/test/int", count++) ? "ok" : fbdo.errorReason().c_str());
        //Firebase.RTDB.setBool(&fbdo, F("/test/iobool"),  true );
        //Firebase.RTDB.setInt(&fbdo, F("/test/int"), 12);
        //Maju
        pembacaan();
        gerak();
        
        
       
   
}
void pembacaan(){
//    Firebase.RTDB.getBool(&fbdo, FPSTR("/test/maju")); 
//    maju = fbdo.to<bool>();
//    Firebase.RTDB.getBool(&fbdo, FPSTR("/test/mundur")); 
//    mundur = fbdo.to<bool>();
//    Firebase.RTDB.getBool(&fbdo, FPSTR("/test/kanan")); 
//    kanan = fbdo.to<bool>();
//    Firebase.RTDB.getBool(&fbdo, FPSTR("/test/kiri")); 
//    mundur = fbdo.to<bool>();
//    Firebase.RTDB.getBool(&fbdo, FPSTR("/test/klakson")); 
//    klakson = fbdo.to<bool>();
//    Firebase.RTDB.getBool(&fbdo, FPSTR("/test/lampu_belakang"));
//    lb = fbdo.to<bool>(); 
//    Firebase.RTDB.getBool(&fbdo, FPSTR("/test/lampu_depan")); 
//    ld = fbdo.to<bool>();
//    Firebase.RTDB.getBool(&fbdo, FPSTR("/test/riting_kanan")); 
//    rkn = fbdo.to<bool>();
//    Firebase.RTDB.getBool(&fbdo, FPSTR("/test/riting_kiri")); 
//    rkr = fbdo.to<bool>();

    Firebase.RTDB.getJSON(&fbdo, "test");
    String cek = fbdo.to<String>();
    //Serial.println(cek); 
    StaticJsonDocument<200> doc;
    deserializeJson(doc, cek);
    JsonObject obj = doc.as<JsonObject>();
    maju = obj["maju"];
    pwm = obj["kecepatan"];
    mundur = obj["mundur"];
    kanan = obj["kanan"];
    kiri = obj["kiri"];
    klakson = obj["klakson"];
    lb = obj["lampu_belakang"];
    ld = obj["lampu_depan"];
    rkn = obj["riting_kanan"];
    rkr = obj["riting_kiri"];
    
//    Serial.println(pwm);
    
  
  }
void gerak(){

      if(maju == true){
          forward();
        
        } else if(mundur == true){
            backward();
          } else if(maju == true && mundur == true){
              mandek1();
            } else {
              mandek1();
              }

      if(kanan == true){
          right();
        
        } else if(kiri == true){
            left();
          } else if(kanan == true && kiri == true) {
              mandek2();
          } else {
              mandek2();
            }
            
      if(ld == true){
        lampu_depan();
        Serial.println("ld");
      }
      else {
        lampu_mati();
        } 
        
      if(lb == true){
        lampu_belakang();
      }
      else{
        lampu_mati();
        }
         
      if(rkn == true){
        riting_kanan();
      }
      else {
        lampu_mati();
        } 
        
      if(rkr == true){
        riting_kiri();
      }
      else{
        lampu_mati();
        }
//      if(klakson == true){
//        buz();
//        } else {
//          digitalWrite(buzzer,LOW);
//          }
}

void mandek1(){
          analogWrite(pwma, 0);
          analogWrite(pwmb, 0);
//          analogWrite(pwmc, 130);
//          analogWrite(pwmd, 0);
  
  
  }
void mandek2(){
          analogWrite(pwmc, 0);
          analogWrite(pwmd, 0);
//          analogWrite(pwmc, 130);
//          analogWrite(pwmd, 0);
  
  
  }
void forward(){
          analogWrite(pwmb, pwm);
          analogWrite(pwma, 0);
          Serial.println("m");
//          analogWrite(pwmc, 130);
//          analogWrite(pwmd, 0);
  
  
  }
void backward(){
          analogWrite(pwmb, 0);
          analogWrite(pwma, pwm);
//          analogWrite(pwmc, 130);
//          analogWrite(pwmd, 0);
  
  
  }
void right(){
//          analogWrite(pwma, pwm);
//          analogWrite(pwmb, 0);
          analogWrite(pwmd, 200);
          analogWrite(pwmc, 0);
  
  
  }
void left(){
//          analogWrite(pwma, pwm);
//          analogWrite(pwmb, 0);
          analogWrite(pwmd, 0);
          analogWrite(pwmc, 200);
  
  
  }
void lampu_depan(){
    
      digitalWrite(lamp_d, HIGH);
      
      
}
      
void lampu_belakang(){
    
      digitalWrite(lamp_b, HIGH);
      
}
      
void riting_kanan(){
    
      digitalWrite(riting_kn, HIGH);
      delay(1000);
      digitalWrite(riting_kn, LOW);
      delay(1000);
      
}

void riting_kiri(){
    
      digitalWrite(riting_kr, HIGH);
      delay(1000);
      digitalWrite(riting_kr, LOW);
      delay(1000);
      
  }
//void buz(){
//  
//  digitalWrite(buzzer,HIGH);
//  
//  }
void lampu_mati(){

  digitalWrite(riting_kr, LOW);
  digitalWrite(riting_kn, LOW);
  digitalWrite(lamp_b, LOW);
  digitalWrite(lamp_d, LOW);
  
  }

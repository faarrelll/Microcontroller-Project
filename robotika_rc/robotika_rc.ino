
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
#define pwma D1  // 16 corresponds to GPIO16
#define pwmb D2 // 17 corresponds to GPIO17
#define pwmc D3  // 5 corresponds to GPIO5
#define pwmd D4  // 5 corresponds to GPIO5
#define buz D5  // 5 corresponds to GPIO5
#define led D6

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Kon"
#define WIFI_PASSWORD "11223344"

/* 2. If work with RTDB, define the RTDB URL and database secret */
#define DATABASE_URL "robotika-b6846-default-rtdb.asia-southeast1.firebasedatabase.app" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "JHejGNm1fPd9VfDczn2lm1ZmUHJINcwepc9cpsRq"

/* 3. Define the Firebase Data object */
FirebaseData fbdo;

/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;
bool maju,kiri,kanan,mundur,klakson,lampu_belakang,lampu_depan,riting_kanan,riting_kiri,buzz,Led;
int pwm,otomatis,moderobot;
bool coba = true;


void setup()
{

    Serial.begin(115200);
    pinMode(pwma, OUTPUT);
    pinMode(pwmc, OUTPUT);
    pinMode(pwmb, OUTPUT);
    pinMode(pwmd, OUTPUT);
    pinMode(buz, OUTPUT);
    pinMode(led, OUTPUT);
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

}

    
void loop()
{
        if(WiFi.status() != WL_CONNECTED){
            Firebase.RTDB.setBool(&fbdo, "/Robot/status", false);
          } else {
            Firebase.RTDB.setBool(&fbdo, "/Robot/status", true);
            } 
        pembacaan();
        pergerakan();
      

  }

 void pembacaan(){
  
//        Firebase.RTDB.getBool(&fbdo, "/Robot/maju"); 
//        maju = fbdo.to<bool>();
//        Firebase.RTDB.getBool(&fbdo, "/Robot/mundur"); 
//        mundur = fbdo.to<bool>();
//        Firebase.RTDB.getBool(&fbdo, "/Robot/kanan"); 
//        kanan = fbdo.to<bool>();
//        Firebase.RTDB.getBool(&fbdo, "/Robot/kiri"); 
//        kiri = fbdo.to<bool>();
//        Firebase.RTDB.getInt(&fbdo, "/Robot/pwm"); 
//        pwm = fbdo.to<int>();

        //Firebase.RTDB.setBool(&fbdo, "/Robot/status", true); 
        Firebase.RTDB.getJSON(&fbdo, "Robot");
        String cek = fbdo.to<String>();
        //Serial.println(cek); 
        StaticJsonDocument<200> doc;
        deserializeJson(doc, cek);
        JsonObject obj = doc.as<JsonObject>();
        maju = obj["maju"];
        pwm = obj["pwm"];
        mundur = obj["mundur"];
        kanan = obj["kanan"];
        kiri = obj["kiri"];
        otomatis = obj["auto"];
        moderobot = obj["mode"];
        
        
//        String output;
//        serializeJson(doc, output);
        Serial.print("auto : ");
        Serial.println(otomatis);
        Serial.print("mode : ");
        Serial.println(moderobot);
        //Serial.println(coba); 
        
  
  }  
void pergerakan(){
  
  if(moderobot == 0){
       if(maju == true)forward();
       else if(mundur == true)backward();
       else if(kanan == true)right();
       else if(kiri == true)left();
       else mandek();
    }
   else if(moderobot == 1){
       if(otomatis == 1)forward();
       else if(otomatis == 2)backward();
       else if(otomatis == 3)right();
       else if(otomatis == 4)left();
       else if (otomatis == 5)mandek();
    }
   
 
}


void forward(){
//          Serial.println("maju");
          analogWrite(pwmb, pwm);
          analogWrite(pwmc, pwm);
          digitalWrite(pwma, LOW);
          digitalWrite(pwmd, LOW);
          
         
          }

void backward(){
//         Serial.println("mundur");
          analogWrite(pwma, pwm);
          analogWrite(pwmd, pwm);
          digitalWrite(pwmb, LOW);
          digitalWrite(pwmc, LOW);
         
          }
          
void right(){
//          Serial.println("kanan");
          analogWrite(pwmb, pwm);
          analogWrite(pwmd, pwm);
          digitalWrite(pwma, LOW);
          digitalWrite(pwmc, LOW);
         
          }
void left(){
          

//          Serial.println("kiri");
          analogWrite(pwma, pwm);
          analogWrite(pwmc, pwm);
          digitalWrite(pwmb, LOW);
          digitalWrite(pwmd, LOW);
         
          }
void mandek(){
         
            analogWrite(pwmb, 0);
            analogWrite(pwmc, 0);
            digitalWrite(pwma, LOW);
            digitalWrite(pwmd, LOW);
            //digitalWrite(buz,HIGH);
            
          }

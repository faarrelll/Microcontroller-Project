
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>


// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>
#define pwma 18  // 16 corresponds to GPIO16
#define pwmb 5 // 17 corresponds to GPIO17
#define pwmc 17  // 5 corresponds to GPIO5
#define pwmd 16  // 5 corresponds to GPIO5

/* 1. Define the WiFi credentials */
#define WIFI_SSID "PLAY MEDIA"
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
bool maju,kiri,kanan,mundur,klakson,lampu_belakang,lampu_depan,riting_kanan,riting_kiri;
int pwm,m;
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;


void setup()
{

    Serial.begin(115200);
    ledcSetup(0, freq, resolution);
    ledcSetup(1, freq, resolution);
    ledcAttachPin(pwma, 0);
    ledcAttachPin(pwmc, 1);
    ledcAttachPin(pwma, 2);
    ledcAttachPin(pwmc, 3);
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

}

    
void loop()
{
    
        pembacaan();
        pergerakan();
      

  }

 void pembacaan(){
  
        Firebase.RTDB.getBool(&fbdo, "/Robot/maju"); 
        maju = fbdo.to<bool>();
        Firebase.RTDB.getBool(&fbdo, "/Robot/mundur"); 
        mundur = fbdo.to<bool>();
        Firebase.RTDB.getBool(&fbdo, "/Robot/kanan"); 
        kanan = fbdo.to<bool>();
        Firebase.RTDB.getBool(&fbdo, "/Robot/kiri"); 
        kiri = fbdo.to<bool>();
        Firebase.RTDB.getInt(&fbdo, "/Robot/pwm"); 
        pwm = fbdo.to<int>();
  
  }  
void pergerakan(){
  
    if(maju == true)forward();
    else if(mundur == true)backward();
    else if(kanan == true)right();
    else if(kiri == true)left();
    else mandek();
 
  }


void forward(){
          Serial.println("maju");
          ledcWrite(0, 100);
          ledcWrite(1, 100);
          digitalWrite(pwmb, LOW);
          digitalWrite(pwmd, LOW);
          }

void backward(){
         Serial.println("mundur");
          ledcWrite(2, 100);
          ledcWrite(3, 100);
          digitalWrite(pwma, LOW);
          digitalWrite(pwmc, LOW);
         
          }
          
void right(){
          Serial.println("kanan");
          ledcWrite(0, 100);
          ledcWrite(1, 0);
          digitalWrite(pwmb, LOW);
          digitalWrite(pwmd, LOW);
         
          }
void left(){
          Serial.println("kiri");
          ledcWrite(0, 0);
          ledcWrite(1, 100);
          digitalWrite(pwmb, LOW);
          digitalWrite(pwmd, LOW);
         
          }
void mandek(){
         
          digitalWrite(pwma, LOW);
          digitalWrite(pwmd, LOW);
          digitalWrite(pwmb, LOW);
            digitalWrite(pwmc, LOW);
            
          }

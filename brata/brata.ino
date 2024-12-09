
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


#define WIFI_SSID "OCEAN"
#define WIFI_PASSWORD "cerdas2023"

#define DATABASE_URL "monitoring-kualitas-air-e9b4c-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "G9j6vwJucVX6fbV5f3CPbCOGhlZl20L9ol5XYmT7"

int turbi = 33;
int potpin = A0;

/* 3. Define the Firebase Data object */
FirebaseData fbdo;

/* 4, Define the FirebaseAuth data for authentication data */
FirebaseAuth auth;

/* Define the FirebaseConfig data for config data */
FirebaseConfig config;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
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

void loop() {
  // put your main code here, to run repeatedly:
  int nilai_ph = analogRead(potpin);
  int nilai_turbi = analogRead(turbi);
  Serial.println(nilai_ph);
  Serial.println(nilai_turbi);
  Firebase.RTDB.setInt(&fbdo, F("/test/ph"), nilai_ph);
  Firebase.RTDB.setInt(&fbdo, F("/test/turbi"), nilai_turbi);
  
}

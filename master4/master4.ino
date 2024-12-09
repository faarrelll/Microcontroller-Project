#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <ArduinoJson.h>
#include <addons/RTDBHelper.h>
#define DATABASE_URL "mobil-listrik-98dbd-default-rtdb.asia-southeast1.firebasedatabase.app"  //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "i1ZZaxiqDAXR7VGep42GLYbxJJFom1520gLaqJFK"


#define WIFI_SSID "OCEAN"
#define WIFI_PASSWORD "cerdas2023"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

TaskHandle_t Task1;
TaskHandle_t Task2;


void setup() {
  Serial.begin(115200); 
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

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(
                    Task1code,   /* Task function. */
                    "Task1",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task1,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */                  
  delay(500); 

  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(
                    Task2code,   /* Task function. */
                    "Task2",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &Task2,      /* Task handle to keep track of created task */
                    1);          /* pin task to core 1 */
    delay(500); 
}

//Task1code: blinks an LED every 1000 ms
void Task1code( void * pvParameters ){
  Serial.print("Task1 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
    // olah data json & kirim ke firebase
  } 
}

//Task2code: blinks an LED every 700 ms
void Task2code( void * pvParameters ){
  Serial.print("Task2 running on core ");
  Serial.println(xPortGetCoreID());

  for(;;){
//  Firebase.RTDB.setInt(&fbdo, F("/test/ph"), nilai_ph);
//  Firebase.RTDB.setInt(&fbdo, F("/test/turbi"), nilai_turbi);

//ambil data di firebase, dan kontrol pompa
  }
}

void loop() {
  
}

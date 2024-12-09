#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <ArduinoJson.h>
#include <addons/RTDBHelper.h>


#define DATABASE_URL "siaptempur-9c860-default-rtdb.asia-southeast1.firebasedatabase.app"  //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "tFuU5xY1ge6aO8d7At0h2O6AEM725bLWidSvQcZf"

#define pwma 33  // maju
#define pwmb 32  // mundur
#define pwmc 27  // kanan
#define pwmd 26  // kiri
#define pwme 25
#define pwmf 23
#define pwmg 19
#define pwmh 18
#define pulbadan 12
#define dirbadan 13
#define pultembak 4
#define dirtembak 2
#define tembakjos 22

const char* ssid = "OCEAN";
const char* password = "cerdas2023";

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

  
bool tembak;
int kecepatan, belok, mode, putarbadan, putartembak, maju, mundur;
int kanan, kiri;


volatile bool dataChanged = false;
int treshold = 130;


int pwma_DutyCycle;
int pwmb_DutyCycle;
int pwmc_DutyCycle;
int pwmd_DutyCycle;
int pwme_DutyCycle;
int pwmf_DutyCycle;
int pwmg_DutyCycle;
int pwmh_DutyCycle;
    bool tembakcoy = false;

// int pulbadan_DutyCycle;
// int pultembak_DutyCycle;

const int pwma_PWMFreq = 5000;
const int pwma_PWMChannel = 0;
const int pwma_PWMResolution = 8;
//const int RED_MAX_DUTY_CYCLE = (int)(pow(2, redPWMResolution) - 1);

const int pwmb_PWMFreq = 5000;
const int pwmb_PWMChannel = 1;
const int pwmb_PWMResolution = 8;
//const int GREEN_MAX_DUTY_CYCLE = (int)(pow(2, greenPWMResolution) - 1);

const int pwmc_PWMFreq = 5000;
const int pwmc_PWMChannel = 2;
const int pwmc_PWMResolution = 8;
//const int BLUE_MAX_DUTY_CYCLE = (int)(pow(2, bluePWMResolution) - 1);

const int pwmd_PWMFreq = 5000;
const int pwmd_PWMChannel = 3;
const int pwmd_PWMResolution = 8;
//const int BLUE_MAX_DUTY_CYCLE = (int)(pow(2, bluePWMResolution) - 1);

const int pwme_PWMFreq = 5000;
const int pwme_PWMChannel = 4;
const int pwme_PWMResolution = 8;
//const int BLUE_MAX_DUTY_CYCLE = (int)(pow(2, bluePWMResolution) - 1);

const int pwmf_PWMFreq = 5000;
const int pwmf_PWMChannel = 5;
const int pwmf_PWMResolution = 8;
//const int BLUE_MAX_DUTY_CYCLE = (int)(pow(2, bluePWMResolution) - 1);

const int pwmg_PWMFreq = 5000;
const int pwmg_PWMChannel = 6;
const int pwmg_PWMResolution = 8;
//const int BLUE_MAX_DUTY_CYCLE = (int)(pow(2, bluePWMResolution) - 1);

const int pwmh_PWMFreq = 5000;
const int pwmh_PWMChannel = 7;
const int pwmh_PWMResolution = 8;
//const int BLUE_MAX_DUTY_CYCLE = (int)(pow(2, bluePWMResolution) - 1);

// const int pulbadan_PWMFreq = 5000;
// const int pulbadan_PWMChannel = 8;
// const int pulbadan_PWMResolution = 8;
//const int BLUE_MAX_DUTY_CYCLE = (int)(pow(2, bluePWMResolution) - 1);

// const int pultembak_PWMFreq = 5000;
// const int pultembak_PWMChannel = 9;
// const int pultembak_PWMResolution = 8;
//const int BLUE_MAX_DUTY_CYCLE = (int)(pow(2, bluePWMResolution) - 1);



// Timer variables

int dwaktu = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(dirbadan, OUTPUT);
  pinMode(pulbadan, OUTPUT);
  pinMode(pultembak, OUTPUT);
  pinMode(dirtembak, OUTPUT);
  pinMode(tembakjos,OUTPUT);
    digitalWrite(tembakjos,HIGH);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connect cok");
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;
  config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h

  // fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 /, 1024 / Tx buffer size in bytes from 512 - 16384 */);

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);
  Firebase.reconnectWiFi(true);
  Firebase.begin(&config, &auth);

  config.timeout.serverResponse = 10 * 1000;

  /* Initialize PWM Channels with Frequency and Resolution */
  ledcSetup(pwma_PWMChannel, pwma_PWMFreq, pwma_PWMResolution);
  ledcSetup(pwmb_PWMChannel, pwmb_PWMFreq, pwmb_PWMResolution);
  ledcSetup(pwmc_PWMChannel, pwmc_PWMFreq, pwmc_PWMResolution);
  ledcSetup(pwmd_PWMChannel, pwmd_PWMFreq, pwmd_PWMResolution);
  ledcSetup(pwme_PWMChannel, pwme_PWMFreq, pwme_PWMResolution);
  ledcSetup(pwmf_PWMChannel, pwmf_PWMFreq, pwmf_PWMResolution);
  ledcSetup(pwmg_PWMChannel, pwmg_PWMFreq, pwmg_PWMResolution);
  ledcSetup(pwmh_PWMChannel, pwmh_PWMFreq, pwmh_PWMResolution);
  // ledcSetup(pulbadan_PWMChannel, pulbadan_PWMFreq, pulbadan_PWMResolution);
  // ledcSetup(pultembak_PWMChannel, pultembak_PWMFreq, pultembak_PWMResolution);

  /* Attach the LED PWM Channel to the GPIO Pin */
  ledcAttachPin(pwma, pwma_PWMChannel);
  ledcAttachPin(pwmb, pwmb_PWMChannel);
  ledcAttachPin(pwmc, pwmc_PWMChannel);
  ledcAttachPin(pwmd, pwmd_PWMChannel);
  ledcAttachPin(pwme, pwme_PWMChannel);
  ledcAttachPin(pwmf, pwmf_PWMChannel);
  ledcAttachPin(pwmg, pwmg_PWMChannel);
  ledcAttachPin(pwmh, pwmh_PWMChannel);
  // ledcAttachPin(pulbadan, pulbadan_PWMChannel);
  // ledcAttachPin(pultembak, pultembak_PWMChannel);
}

void loop() {

  if (WiFi.status() == WL_CONNECTED) {
    //initWiFi();
    // if(Firebase.failed()){
    //   Serial.println("gara2 iki");
    // }
    firebase();
  } else {
    Serial.print(".");
  }
}

void firebase() {

  Firebase.RTDB.getJSON(&fbdo, "tank");
  String cek = fbdo.to<String>();
  //Serial.println(cek);
  DynamicJsonDocument doc(4096);
  // StaticJsonDocument<4096> doc;
  deserializeJson(doc, cek);
  JsonObject obj = doc.as<JsonObject>();
  belok = obj["belok"];
  kecepatan = obj["kecepatan"];
  mode = obj["mode"];
  putarbadan = obj["putarbadan"];
  putartembak = obj["putartembak"];
  tembak = obj["tembak"];

  // Serial.println(cek);
  if (mode == 1) {
    if (kecepatan >= 0) {
      mundur = 0;
      maju = kecepatan;
      //    digitalWrite(lampu_belakang, HIGH);
      //    digitalWrite(lampu_belakang, LOW);
    } else {
      mundur = kecepatan * -1;
      maju = 0;
      //    digitalWrite(lampu_belakang, LOW);
    }
    if (belok >= 0) {
      kiri = 0;
      kanan = belok;
      //    digitalWrite(lampu_belakang, HIGH);
      //    digitalWrite(lampu_belakang, LOW);
    } else {
      kanan = 0;
      kiri = belok * -1;

      //    digitalWrite(lampu_belakang, LOW);
    }

    ledcWrite(pwma_PWMChannel, maju);
    ledcWrite(pwmb_PWMChannel, mundur);
    ledcWrite(pwme_PWMChannel, mundur);
    ledcWrite(pwmf_PWMChannel, maju);

    //kiri kanan
    ledcWrite(pwmc_PWMChannel, kanan);
    ledcWrite(pwmd_PWMChannel, kiri);
    ledcWrite(pwmg_PWMChannel, kiri);
    ledcWrite(pwmh_PWMChannel, kanan);

  } else if (mode == 2) {

    if (kecepatan >= 0) {
      mundur = 0;
      maju = kecepatan;
      //    digitalWrite(lampu_belakang, HIGH);
      //    digitalWrite(lampu_belakang, LOW);
    } else {
      mundur = kecepatan * -1;
      maju = 0;
      //    digitalWrite(lampu_belakang, LOW);
    }
    if (belok >= 0) {
      kiri = 0;
      kanan = belok;
      //    digitalWrite(lampu_belakang, HIGH);
      //    digitalWrite(lampu_belakang, LOW);
    } else {
      kanan = 0;
      kiri = belok * -1;

      //    digitalWrite(lampu_belakang, LOW);
    }
    ledcWrite(pwma_PWMChannel, maju);
    ledcWrite(pwmb_PWMChannel, mundur);
    ledcWrite(pwme_PWMChannel, maju);
    ledcWrite(pwmf_PWMChannel, mundur);
    ledcWrite(pwmc_PWMChannel, kanan);
    ledcWrite(pwmd_PWMChannel, kiri);
    ledcWrite(pwmg_PWMChannel, kanan);
    ledcWrite(pwmh_PWMChannel, kiri);
    Serial.print("maju : ");
    Serial.println(maju);
    Serial.print("mundur : ");
    Serial.println(mundur);
    Serial.print("kanan : ");
    Serial.println(kanan);
    Serial.print("kiri : ");
    Serial.println(kiri);

  } else if (mode == 3) {
    int sudut = 0;
    if (putarbadan >= 0) {
      if (putarbadan >= 360) {
        putarbadan = 360;
      }
      sudut = map(putarbadan, 0, 360, 0, 17500);
      Serial.print("sudut :");
      Serial.println(sudut);
      for (int i = 0; i < sudut; i++)  // Forward 5000 steps
      {
        digitalWrite(dirbadan, LOW);
        // digitalWrite(ENA, HIGH);
        digitalWrite(pulbadan, HIGH);
        delayMicroseconds(500);
        digitalWrite(pulbadan, LOW);
        delayMicroseconds(500);
      }
      // for (int i = 0; i <= sudut; i++) {
      //   ledcWrite(pulbadan_PWMChannel, 100);
      //   digitalWrite(dirbadan, LOW);
      //   delay(10);
      // }
      // ledcWrite(pulbadan_PWMChannel, 0);
      Firebase.RTDB.setInt(&fbdo, "/tank/mode", 0);
      Firebase.RTDB.setInt(&fbdo, "/tank/putarbadan", 0);
    } else if (putarbadan < 0) {
      putarbadan = putarbadan * -1;
      if (putarbadan >= 360) {
        putarbadan = 360;
      }
      sudut = map(putarbadan, 0, 360, 0, 17500);
      for (int i = 0; i < sudut; i++)  // Forward 5000 steps
      {
        digitalWrite(dirbadan, HIGH);
        // digitalWrite(ENA, HIGH);
        digitalWrite(pulbadan, HIGH);
        delayMicroseconds(500);
        digitalWrite(pulbadan, LOW);
        delayMicroseconds(500);
      }
      Firebase.RTDB.setInt(&fbdo, "/tank/mode", 0);
      Firebase.RTDB.setInt(&fbdo, "/tank/putarbadan", 0);
    }
    sudut = 0;

    if (putartembak >= 0) {
      if (putartembak >= 45) {
        putartembak = 45;
      }
      sudut = map(putartembak, 0, 360, 0, 17500);
      for (int i = 0; i < sudut; i++)  // Forward 5000 steps
      {
        digitalWrite(dirtembak, HIGH);
        digitalWrite(pultembak, HIGH);
        delayMicroseconds(500);
        digitalWrite(pultembak, LOW);
        delayMicroseconds(500);
      }
      Firebase.RTDB.setInt(&fbdo, "/tank/mode", 0);
      Firebase.RTDB.setInt(&fbdo, "/tank/putartembak", 0);
    } else if (putartembak < 0) {
      putartembak = putartembak * -1;
      if (putartembak >= 45) {
        putartembak = 45;
      }
      sudut = map(putartembak, 0, 360, 0, 17500);
      for (int i = 0; i < sudut; i++)  // Forward 5000 steps
      {
        digitalWrite(dirtembak, LOW);
        digitalWrite(pultembak, HIGH);
        delayMicroseconds(500);
        digitalWrite(pultembak, LOW);
        delayMicroseconds(500);
      }
      Firebase.RTDB.setInt(&fbdo, "/tank/mode", 0);
      Firebase.RTDB.setInt(&fbdo, "/tank/putartembak", 0);
    }
  }
  
  if(tembak == 1){
    if(millis()-dwaktu >= 50){
      if(tembakcoy == true){
        tembakcoy = false;
      }else if (tembakcoy == false){
        tembakcoy = true;
      }
      dwaktu = millis();
      Serial.println("fire cok");
//              digitalWrite(tembakjos,LOW);
      }
      if(tembakcoy == true){
      Serial.println("Nyala");
              digitalWrite(tembakjos,LOW);
      }else if(tembakcoy == false){
      Serial.println("Mati");
              digitalWrite(tembakjos,HIGH);
      }

      
//     digitalWrite(tembakjos,LOW); 
    } else {
    digitalWrite(tembakjos,HIGH);
      }


  doc.clear();

//  delay(300);
}

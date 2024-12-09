#ifndef FIREBASE_H
#define FIREBASE_H
#include <Arduino.h>

#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <ArduinoJson.h>
#include <addons/RTDBHelper.h>

#define DATABASE_URL "mobil-listrik-98dbd-default-rtdb.asia-southeast1.firebasedatabase.app"  //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "i1ZZaxiqDAXR7VGep42GLYbxJJFom1520gLaqJFK"
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

class fb{

  private:
    bool satu, dua, tiga, empat, lima;
    int kecepatan, belok, maju, mundur, kanan, kiri;
    int sein_kiri = 26;
    int sein_kanan = 18;
    int  klakson = 19;
    int  lampu_utama = 23;
    int  lampu_belakang = 25;

    volatile bool dataChanged = false;
    int treshold = 100;

    int pwma,pwmb,pwmc,pwmd;
    int pwma_DutyCycle;
    int pwmb_DutyCycle;
    int pwmc_DutyCycle;
    int pwmd_DutyCycle;
    
    const int pwma_PWMFreq = 30000;
    const int pwma_PWMChannel = 0;
    const int pwma_PWMResolution = 8;
    //const int RED_MAX_DUTY_CYCLE = (int)(pow(2, redPWMResolution) - 1);
    
    const int pwmb_PWMFreq = 30000;
    const int pwmb_PWMChannel = 1;
    const int pwmb_PWMResolution = 8;
    //const int GREEN_MAX_DUTY_CYCLE = (int)(pow(2, greenPWMResolution) - 1);
    
    const int pwmc_PWMFreq = 30000;
    const int pwmc_PWMChannel = 2;
    const int pwmc_PWMResolution = 8;
    //const int BLUE_MAX_DUTY_CYCLE = (int)(pow(2, bluePWMResolution) - 1);
    
    const int pwmd_PWMFreq = 30000;
    const int pwmd_PWMChannel = 3;
    const int pwmd_PWMResolution = 8;
    //const int BLUE_MAX_DUTY_CYCLE = (int)(pow(2, bluePWMResolution) - 1);
   
    
  public:

  fb() {}
  //, int e, int f, int g, int h
  fb(int a, int b, int c, int d){
   
   pwma = a;
   pwmb = b;
   pwmc = c;
   pwmd = d;
  //  sein_kiri = e;
  //  sein_kanan = f;
  //  klakson = g;
  //  lampu_utama = h;
  //  lampu_belakang = i;
   
  }

  void init(){

  pinMode(pwma, OUTPUT);
  pinMode(pwmb, OUTPUT);
  pinMode(pwmc, OUTPUT);
  pinMode(pwmd, OUTPUT);
  pinMode(sein_kiri, OUTPUT);
  pinMode(sein_kanan, OUTPUT);
  pinMode(klakson, OUTPUT);
  pinMode(lampu_utama, OUTPUT);
  pinMode(lampu_belakang, OUTPUT);
  
  
  /* Initialize PWM Channels with Frequency and Resolution */
  ledcSetup(pwma_PWMChannel, pwma_PWMFreq, pwma_PWMResolution);
  ledcSetup(pwmb_PWMChannel, pwmb_PWMFreq, pwmb_PWMResolution);
  ledcSetup(pwmc_PWMChannel, pwmc_PWMFreq, pwmc_PWMResolution);
  ledcSetup(pwmd_PWMChannel, pwmd_PWMFreq, pwmd_PWMResolution);
  /* Attach the LED PWM Channel to the GPIO Pin */
  ledcAttachPin(pwma, pwma_PWMChannel);
  ledcAttachPin(pwmb, pwmb_PWMChannel);
  ledcAttachPin(pwmc, pwmc_PWMChannel);
  ledcAttachPin(pwmd, pwmd_PWMChannel);
 
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;
  
  Firebase.reconnectWiFi(true);
  Firebase.begin(&config, &auth);
  
  }
  void get_data()
  {

  Firebase.RTDB.getJSON(&fbdo, "mobil");
  String cek = fbdo.to<String>();
  //Serial.println(cek);
  StaticJsonDocument<1024> doc;
  deserializeJson(doc, cek);
  JsonObject obj = doc.as<JsonObject>();
  kecepatan = obj["kecepatan"];
  belok = obj["belok"];
  dua = obj["seinkanan"];
  satu = obj["seinkiri"];
  tiga = obj["klakson"];
  empat = obj["lampuutama"];
  lima = obj["lampubelakang"];
  Serial.println(cek);

  if (kecepatan >= 0) {
    mundur = 0;
    maju = kecepatan;
  } else {
    mundur = kecepatan * -1;
    maju = 0;
  }
  if (belok >= 0) {
    kiri = 0;
    if (belok >= treshold) {
      belok = treshold;
    }
    kanan = belok;
  } else {
    kiri = belok * -1;
    if (kiri >= treshold) {
      kiri = treshold;
    }
    kanan = 0;
  }

  digitalWrite(sein_kiri, satu);
  digitalWrite(sein_kanan, dua);
  digitalWrite(klakson, tiga);
  digitalWrite(lampu_utama, empat);
  digitalWrite(lampu_belakang, lima);
  
  
  }

  void gerak(){

  ledcWrite(pwma_PWMChannel, maju);
  ledcWrite(pwmb_PWMChannel, mundur);
  ledcWrite(pwmc_PWMChannel, kanan);
  ledcWrite(pwmd_PWMChannel, kiri);
 
  }
  
  };

#endif

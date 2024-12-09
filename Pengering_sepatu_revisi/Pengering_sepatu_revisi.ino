#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFiManager.h>
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <Arduino_JSON.h>

#define DHTTYPE DHT22

#define DHTPIN1 4  //kanan   
#define DHTPIN2 16 // atas
#define DHTPIN3 33 // kiri
#define DHTPIN4 17 // atas
#define DHTPIN5 32 // kiri

#define bt_kanan 13 // merah
#define bt_tengah 14 // kuning
#define bt_kiri 27 // biru

AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Json Variable to Hold Sensor Readings
JSONVar readings;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 1000;
unsigned long startTime = 0; // Waktu mulai stopwatch dalam 
unsigned long elapsedTime;
unsigned long currentTime;

bool isStopwatchRunning = false;
bool isServerRunning = false;
String stopwatchTime;

int pilih = 0;
int menu = 1;
float esuhu = 0;
float dsuhu = 0;
float EN,N,EP,KB,KK,Z,PK,PB;
float rule[15]={};
float z_pemanas[15]={};
float z_kipas[15]={};
int pwm_kipas, pwm_pemanas;
int awal =0;
int awal2 =0;
int selesai = 0;
float kelembapan;
float suhu;
String status;
String jenis;
int indikator_merah = 15;
int indikator_biru = 12;

LiquidCrystal_I2C lcd(0x27, 20, 4);
DHT dht1(DHTPIN1, DHTTYPE); // kanan
DHT dht2(DHTPIN2, DHTTYPE); // atas
DHT dht3(DHTPIN3, DHTTYPE); // kiri
DHT dht4(DHTPIN4, DHTTYPE); 
DHT dht5(DHTPIN5, DHTTYPE); 

const int Fan = 23; //inhouse
const int Fan2 = 25; // fan heater 
const int Fan3 = 26; // exhouse

const int freq = 20000;
const int ledChannel = 0;
const int ledChannel2 = 1;
const int ledChannel3 = 2;
const int resolution = 8;

int bst_lpwm = 18;
int bst_rpwm = 19;  
int channel = 4;
int channel2 = 5;
int freq2  = 1000;

String getSensorReadings(){
  readings["temperature"] = suhu;
  readings["humidity"] =  kelembapan;
  readings["jenisSepatu"] =  "Knit";
  readings["waktu"] =  getStopwatchTime();
  readings["status"] =  status;
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

String getSensorReadings2(){
  readings["temperature"] = suhu;
  readings["humidity"] =  kelembapan;
  readings["jenisSepatu"] =  "Meshd";
  readings["waktu"] =  getStopwatchTime();
  readings["status"] =  status;
  String jsonString = JSON.stringify(readings);
  return jsonString;
}
//String getSensorReadings3(){
//  readings["temperature"] = suhu;
//  readings["humidity"] =  kelembapan;
//  readings["jenisSepatu"] =  "Fast Drying";
//  readings["waktu"] =  getStopwatchTime();
//  readings["status"] =  status;
//  String jsonString = JSON.stringify(readings);
//  return jsonString;
//}

// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

void initWiFi() {
//  WiFi.mode(WIFI_STA);
    loading();
    lcd.setCursor(1, 1);
    lcd.print("Connect WIfi !!!");
    lcd.setCursor(1, 2);
    lcd.print("=> Pengering Sepatu");
    WiFiManager wm;
    bool res;
    res = wm.autoConnect("Pengering Sepatu","12345678"); // password protected ap
    if(!res) {
        Serial.println("Failed to connect");
         ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
        lcd.setCursor(1, 1);
        lcd.print("WiFi Connected");
        delay(1000);
        hapus_antarmuka();
    }
}

void setup() {
  lcd.begin(); // initialize the lcd
  lcd.backlight();
  Serial.begin(115200);
  dht1.begin();
  dht2.begin();
  dht3.begin();
   dht4.begin();
  dht5.begin();
  initWiFi();
  initSPIFFS();
  
  ledcSetup(ledChannel, freq, resolution);
  ledcSetup(ledChannel2, freq, resolution);
  ledcSetup(ledChannel3, freq, resolution);

  ledcSetup(channel, freq2, resolution);
  ledcSetup(channel2, freq2, resolution);
  
  ledcAttachPin(bst_lpwm,channel);
  ledcAttachPin(bst_rpwm,channel2);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(Fan, ledChannel);
  ledcAttachPin(Fan2, ledChannel2);
  ledcAttachPin(Fan3, ledChannel3);
  
  pinMode(bt_kanan, INPUT_PULLUP);
  pinMode(bt_tengah, INPUT_PULLUP);
  pinMode(bt_kiri, INPUT_PULLUP);
  pinMode(indikator_merah, OUTPUT);
  pinMode(indikator_biru, OUTPUT);
  digitalWrite(indikator_merah,HIGH);

    // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = getSensorReadings();
    request->send(200, "application/json", json);
    json = String();
  });

  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  // Start server
//  server.begin();
//  startTime = millis();  

  
}

void loop() {
  
  int btn_menu = digitalRead(bt_kiri);
  int btn_pilih = digitalRead(bt_tengah);
  int btn_batal = digitalRead(bt_kanan);

  antarmuka();

  if (btn_menu == LOW) {
    hapus_antarmuka();
     if (pilih == 0 || pilih == 1) {
      if (menu == 2) {
        menu = 1;
      } else {
        menu = menu + 1;
      }
    }
  } else if (btn_pilih == LOW) {
    hapus_antarmuka();
    
    if (menu == 1 && pilih == 1){
      pilih = 3;
      }
    else if (menu == 2 && pilih == 1){
      pilih = 4;
      }
//    else if (menu == 3 && pilih == 1){
//      pilih = 5;
//      }
    else if (menu == 1 && pilih == 3){
      menu = menu;
    }
     else if(menu == 2 && pilih == 4){
      
        menu = menu;
    } 
    else if (menu == 1) {
      menu = 1;
      pilih = 1;
    } else if (menu == 2) {
      pilih = 2;
    }
  } else if (btn_batal == LOW) {
    hapus_antarmuka();
    if(pilih == 3 || pilih ==4){
      stopServer();
      elapsedTime = 0;
      currentTime =0;
      pilih = 1;
      awal = 0;
      awal2 = 0;
      selesai = 0;
      digitalWrite(indikator_merah,HIGH);
      digitalWrite(indikator_biru,LOW);
      ledcWrite(ledChannel, 0);
      ledcWrite(ledChannel2, 0);
      ledcWrite(ledChannel3, 0);
      ledcWrite(channel, 0);
      ledcWrite(channel2,0);
     } else {
      pilih = 0;
     }
    
  }
}

void antarmuka() {
  if (pilih == 0) {
    if (menu == 0) {
      lcd.setCursor(8, 0);
      lcd.print("MENU");
      lcd.setCursor(0, 1);
      lcd.print("  Pengerigan Sepatu");
      lcd.setCursor(0, 2);
      lcd.print("  Cek Parameter");
    } else if (menu == 1) {
      lcd.setCursor(8, 0);
      lcd.print("MENU");
      lcd.setCursor(0, 1);
      lcd.print("> Pengerigan Sepatu");
      lcd.setCursor(0, 2);
      lcd.print("  Cek Parameter");
    } else if (menu == 2) {
      lcd.setCursor(8, 0);
      lcd.print("MENU");
      lcd.setCursor(0, 1);
      lcd.print("  Pengerigan Sepatu");
      lcd.setCursor(0, 2);
      lcd.print("> Cek Parameter");
    }
  } else if (pilih == 1) {
    if(menu == 0){
      lcd.setCursor(4, 0);
      lcd.print("PENGERINGAN");
      lcd.setCursor(0, 1);
      lcd.print("  Sepatu Knit");
      lcd.setCursor(0, 2);
      lcd.print("  Sepatu Mesh");
    }
    else if(menu == 1){
      lcd.setCursor(4, 0);
      lcd.print("PENGERINGAN");
      lcd.setCursor(0, 1);
      lcd.print("> Sepatu Knit");
      lcd.setCursor(0, 2);
      lcd.print("  Sepatu Mesh");
//      lcd.setCursor(0, 3);
//      lcd.print("  Fast Drying");
      
    }
    else if(menu == 2){
      lcd.setCursor(4, 0);
      lcd.print("PENGERINGAN");
      lcd.setCursor(0, 1);
      lcd.print("  Sepatu Knit");
      lcd.setCursor(0, 2);
      lcd.print("> Sepatu Mesh");
//      lcd.setCursor(0, 3);
//      lcd.print("  Fast Drying");
      
    }
//    else if(menu == 3){
//      lcd.setCursor(4, 0);
//      lcd.print("PENGERINGAN");
//      lcd.setCursor(0, 1);
//      lcd.print("  Sepatu Casual");
//      lcd.setCursor(0, 2);
//      lcd.print("  Sepatu Running");
//      lcd.setCursor(0, 3);
//      lcd.print("> Fast Drying");
//      
//    }
  } 
  else if (pilih == 2) {
    lcd.setCursor(5, 0);
    lcd.print("PARAMETER");
    suhu_tampil();
  } 
  else if (pilih == 3){
    if(awal2 == 0){
      startServer();
      startStopwatch();
      awal2 = 1;
      digitalWrite(indikator_merah,LOW);
      digitalWrite(indikator_biru,HIGH);
      }
    float humi1  = dht1.readHumidity();    // read humidity
    float tempC2 = dht2.readTemperature(); // read temperature
    float humi3  = dht3.readHumidity();    // read humidity
    float humi4  = dht4.readHumidity();
    float humi5  = dht5.readHumidity();
    suhu = tempC2;
    humi3 = humi3-10;
    int set_casual = 35;
    int set_kelembapan = 68;
    esuhu = set_casual - tempC2;
    dsuhu = esuhu - dsuhu;
    kelembapan = (humi1 + humi4 + humi5+ humi3)/4;
    status = "Dalam Proses Pengeringan";

    if ((millis() - lastTime) > timerDelay) {
    // Send Events to the client with the Sensor Readings Every 10 seconds
    events.send("ping",NULL,millis());
    events.send(getSensorReadings().c_str(),"new_readings" ,millis());
    lastTime = millis();

  }
  
    if (kelembapan > set_kelembapan) {
      selesai = 0;
    if (esuhu >4){
        if(awal == 0){
           ledcWrite(ledChannel, 255);
            ledcWrite(ledChannel2, 255);
            ledcWrite(ledChannel3, 255);
            ledcWrite(channel, 255);
            ledcWrite(channel2,0);
            awal = 1;
          } else {
            ledcWrite(ledChannel, 170);
            ledcWrite(ledChannel2, 255);
            ledcWrite(ledChannel3, 170);
            ledcWrite(channel, 255);
            ledcWrite(channel2,0);
            }
       
    } else if(esuhu <= 4 && esuhu >= -4){
       fuzzyfikasi();
       fuzzy_inferensi();
       ledcWrite(ledChannel, pwm_kipas);
        ledcWrite(ledChannel2, pwm_kipas);
        ledcWrite(ledChannel3, pwm_kipas);
        ledcWrite(channel, pwm_pemanas);
      } else if(esuhu < -4){
        ledcWrite(ledChannel, 255);
        ledcWrite(ledChannel2, 255);
        ledcWrite(ledChannel3, 255);
        ledcWrite(channel, 0);
        ledcWrite(channel2,0);
        }
         lcd.setCursor(1,0);
          lcd.print("PENGERINGAN SEPATU");
          lcd.setCursor(0,1);
          lcd.print("Jenis = Knit");
          lcd.setCursor(0, 2);
          lcd.print("S= ");
          lcd.print(tempC2);
          lcd.print("C");
          lcd.print(" K= ");
          lcd.print(kelembapan);
          lcd.print("%");
          lcd.setCursor(0, 3);
          lcd.print("IP:");
          lcd.print(WiFi.localIP()); 
    } else {
        if(selesai == 0){
            stopStopwatch();
            hapus_antarmuka();
            status = "Pengeringan Selesai";
            events.send("ping",NULL,millis());
            events.send(getSensorReadings().c_str(),"new_readings" ,millis());
            selesai = 1;
            digitalWrite(indikator_merah,HIGH);
            digitalWrite(indikator_biru,HIGH);
            ledcWrite(ledChannel, 0);
            ledcWrite(ledChannel2, 0);
            ledcWrite(ledChannel3, 0);
            ledcWrite(channel, 0);
            ledcWrite(channel2,0);
            }
          lcd.setCursor(1,0);
          lcd.print("PENGERINGAN SEPATU");
          lcd.setCursor(0,1);
          lcd.print("Jenis = Knit");
          lcd.setCursor(0, 2);
          lcd.print("       SELESAI");
      
      }
    }
   else if (pilih == 4){
    if(awal2 == 0){
      startServer();
      startStopwatch();
      awal2 = 1;
      digitalWrite(indikator_merah,LOW);
      digitalWrite(indikator_biru,HIGH);
      }
    float humi1  = dht1.readHumidity();    // read humidity
    float tempC2 = dht2.readTemperature(); // read temperature
    float humi3  = dht3.readHumidity();    // read humidity
    float humi4  = dht4.readHumidity();
    float humi5  = dht5.readHumidity();
    suhu = tempC2;
    humi3 = humi3-10;
    int set_running = 35;
    int set_kelembapan = 65;
    esuhu = set_running - tempC2;
    dsuhu = esuhu - dsuhu;
    kelembapan = (humi1 + humi4 + humi5+ humi3)/4;
    status = "Dalam Proses Pengeringan";

     if ((millis() - lastTime) > timerDelay) {
    // Send Events to the client with the Sensor Readings Every 10 seconds
    events.send("ping",NULL,millis());
    events.send(getSensorReadings2().c_str(),"new_readings" ,millis());
    lastTime = millis();

  }
  
    if (kelembapan > set_kelembapan) {
      selesai = 0;
    if (esuhu >4){
         if(awal == 0){
           ledcWrite(ledChannel, 255);
            ledcWrite(ledChannel2, 255);
            ledcWrite(ledChannel3, 255);
            ledcWrite(channel, 255);
            ledcWrite(channel2,0);
            awal = 1;
          } else {
            ledcWrite(ledChannel, 225);
            ledcWrite(ledChannel2, 255);
            ledcWrite(ledChannel3, 225);
            ledcWrite(channel, 255);
            ledcWrite(channel2,0);
            }
        
    } else if(esuhu <= 4 && esuhu >= -4){
       fuzzyfikasi();
       fuzzy_inferensi();
       ledcWrite(ledChannel, pwm_kipas);
        ledcWrite(ledChannel2, pwm_kipas);
        ledcWrite(ledChannel3, pwm_kipas);
        ledcWrite(channel, pwm_pemanas);
        
      } else if(esuhu < -4){
        ledcWrite(ledChannel, 255);
        ledcWrite(ledChannel2, 255);
        ledcWrite(ledChannel3, 255);
        ledcWrite(channel, 0);
        ledcWrite(channel2,0);
        }
          lcd.setCursor(1,0);
          lcd.print("PENGERINGAN SEPATU");
          lcd.setCursor(0,1);
          lcd.print("Jenis = Mesh");
          lcd.setCursor(0, 2);
          lcd.print("S= ");
          lcd.print(tempC2);
          lcd.print("C");
          lcd.print(" K= ");
          lcd.print(kelembapan);
          lcd.print("%");
          lcd.setCursor(0, 3);
          lcd.print("IP:");
          lcd.print(WiFi.localIP()); 
    } else {
          
        if(selesai == 0){
             stopStopwatch();
            hapus_antarmuka();
            status = "Pengeringan Selesai";
            events.send("ping",NULL,millis());
            events.send(getSensorReadings2().c_str(),"new_readings2" ,millis());
            selesai = 1;
            digitalWrite(indikator_merah,HIGH);
            digitalWrite(indikator_biru,HIGH);
            ledcWrite(ledChannel, 0);
            ledcWrite(ledChannel2, 0);
            ledcWrite(ledChannel3, 0);
            ledcWrite(channel, 0);
            ledcWrite(channel2,0);
            }
          lcd.setCursor(1,0);
          lcd.print("PENGERINGAN SEPATU");
          lcd.setCursor(0,1);
          lcd.print("Jenis = Mesh");
          lcd.setCursor(0, 2);
          lcd.print("       SELESAI");
      
      }

    }
//    else if(pilih == 5){
//      
//        if(awal2 == 0){
//      startServer();
//      startStopwatch();
//      awal2 = 1;
//      digitalWrite(indikator_merah,LOW);
//      digitalWrite(indikator_biru,HIGH);
//      }
//    float humi1  = dht1.readHumidity();    // read humidity
//    float tempC2 = dht2.readTemperature(); // read temperature
//    float humi3  = dht3.readHumidity();    // read humidity
//    float humi4  = dht4.readHumidity();
//    float humi5  = dht5.readHumidity();
//    suhu = tempC2;
//    humi3 = humi3-10;
//    int set_running = 40;
//    int set_kelembapan = 60;
//    esuhu = set_running - tempC2;
//    dsuhu = esuhu - dsuhu;
//    kelembapan = (humi1 + humi4 + humi5+ humi3)/4;
//    status = "Dalam Proses Pengeringan";
//
//     if ((millis() - lastTime) > timerDelay) {
//    // Send Events to the client with the Sensor Readings Every 10 seconds
//    events.send("ping",NULL,millis());
//    events.send(getSensorReadings3().c_str(),"new_readings" ,millis());
//    lastTime = millis();
//
//  }
//  
//    if (kelembapan > set_kelembapan) {
//      selesai = 0;
//    if (esuhu >4){
//         if(awal == 0){
//           ledcWrite(ledChannel, 255);
//            ledcWrite(ledChannel2, 255);
//            ledcWrite(ledChannel3, 255);
//            ledcWrite(channel, 255);
//            ledcWrite(channel2,0);
//            awal = 1;
//          } else {
//            ledcWrite(ledChannel, 225);
//            ledcWrite(ledChannel2, 255);
//            ledcWrite(ledChannel3, 225);
//            ledcWrite(channel, 255);
//            ledcWrite(channel2,0);
//            }
//        
//    } else if(esuhu <= 4 && esuhu >= -4){
//       fuzzyfikasi();
//       fuzzy_inferensi();
//       ledcWrite(ledChannel, pwm_kipas);
//        ledcWrite(ledChannel2, pwm_kipas);
//        ledcWrite(ledChannel3, pwm_kipas);
//        ledcWrite(channel, pwm_pemanas);
//        
//      } else if(esuhu < -4){
//        ledcWrite(ledChannel, 255);
//        ledcWrite(ledChannel2, 255);
//        ledcWrite(ledChannel3, 255);
//        ledcWrite(channel, 0);
//        ledcWrite(channel2,0);
//        }
//          lcd.setCursor(1,0);
//          lcd.print("PENGERINGAN SEPATU");
//          lcd.setCursor(0,1);
//          lcd.print("Jenis = Fast Drying");
//          lcd.setCursor(0, 2);
//          lcd.print("S= ");
//          lcd.print(tempC2);
//          lcd.print("C");
//          lcd.print(" K= ");
//          lcd.print(kelembapan);
//          lcd.print("%");
//          lcd.setCursor(0, 3);
//          lcd.print("IP:");
//          lcd.print(WiFi.localIP()); 
//    } else {
//          
//        if(selesai == 0){
//             stopStopwatch();
//            hapus_antarmuka();
//            status = "Pengeringan Selesai";
//            events.send("ping",NULL,millis());
//            events.send(getSensorReadings3().c_str(),"new_readings2" ,millis());
//            selesai = 1;
//            digitalWrite(indikator_merah,HIGH);
//            digitalWrite(indikator_biru,HIGH);
//            ledcWrite(ledChannel, 0);
//            ledcWrite(ledChannel2, 0);
//            ledcWrite(ledChannel3, 0);
//            ledcWrite(channel, 0);
//            ledcWrite(channel2,0);
//            }
//          lcd.setCursor(1,0);
//          lcd.print("PENGERINGAN SEPATU");
//          lcd.setCursor(0,1);
//          lcd.print("Jenis = Fast Dryng");
//          lcd.setCursor(0, 2);
//          lcd.print("       SELESAI");
//      
//      }
//      } 
}

void loading() {
  lcd.setCursor(2, 1);
  lcd.print("System Loading..");
  for (int i = 0; i < 20; i++) {
    lcd.setCursor(i, 2);
    lcd.print("*");
    delay(100); // Atur kecepatan loading
  }
  delay(1000);

  lcd.setCursor(0, 2);
  for (int i = 0; i < 20; i++) {
    lcd.print(" ");
  }
  lcd.setCursor(0, 1);
  for (int i = 0; i < 20; i++) {
    lcd.print(" ");
  }

//  // Tampilkan pesan loading selesai
//  lcd.setCursor(1, 1);
//  lcd.print("Loading Complete!!");

  // Tunggu sebentar setelah pesan loading selesai ditampilkan
//  delay(3000);

  // Hapus pesan loading selesai
//  lcd.setCursor(0, 1);
//  for (int i = 0; i < 20; i++) {
//    lcd.print(" ");
//  }
}

void hapus_antarmuka() {
  // hapus tampilan
  lcd.setCursor(0, 0);
  for (int a = 0; a < 20; a++) {
    lcd.print(" ");
  }
  lcd.setCursor(0, 1);
  for (int a = 0; a < 20; a++) {
    lcd.print(" ");
  }
  lcd.setCursor(0, 2);
  for (int a = 0; a < 20; a++) {
    lcd.print(" ");
  }
  lcd.setCursor(0, 3);
  for (int a = 0; a < 20; a++) {
    lcd.print(" ");
  }
}

void suhu_tampil(){
  
  float humi1  = dht1.readHumidity();    // read humidity
//  float tempC1 = dht1.readTemperature(); // read temperature
//  float humi2  = dht2.readHumidity();    // read humidity
  float tempC2 = dht2.readTemperature(); // read temperature
  float humi3  = dht3.readHumidity();    // read humidity
//  float tempC3 = dht3.readTemperature(); // read temperature
  float humi4  = dht4.readHumidity();    // read humidity
  float humi5  = dht5.readHumidity();    // read humidity

  humi3 = humi3-10;
  humi4 = humi4-5;
  humi5 = humi5-5;
  
  if (isnan(tempC2)) {
    lcd.setCursor(0, 1);
    lcd.print("Failed");
  } else {
    lcd.setCursor(0, 1);  // display position
    lcd.print("Suhu:");
    lcd.print(tempC2);     // display the temperature
    lcd.print("C");
    lcd.print(" ");
   // display position
  }

  if (isnan(humi1) || isnan(humi3)) {
    lcd.setCursor(0, 2);
    lcd.print("Failed");
  } else {
    lcd.setCursor(0, 2);  // display position
    lcd.print("L1:");
    lcd.print(humi1);     // display the temperature
    lcd.print("%");
    lcd.print(" ");
  // display position
    lcd.print("L2:");
    lcd.print(humi3);      // display the humidity
    lcd.print("%");
  }

  if (isnan(humi4) || isnan(humi5)) {
    lcd.setCursor(0, 3);
    lcd.print("Failed");
  } else {
    lcd.setCursor(0, 3);  // display position
    lcd.print("L3:");
    lcd.print(humi4);     // display the temperature
    lcd.print("%");
    lcd.print(" ");
      // display position 
    lcd.print("L4:");
    lcd.print(humi5);      // display the humidity
    lcd.print("%");
  }
  }

  //---------------------------------------------FUZZYFIKASI--------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------Fuzzy Input Error_Suhu---------------------------------------------------------------------------------------------------------------------
//linier turun
float fuzzyfikasi_EN(float x){
    float val;
    if(x <= -3){
      val = 1;
      return val;
      } 
    else if(x <= 0 && x >= -3 ) {
      val = (0-(x))/(3);
      return val;
      }
    else if(x >= 0){
      val = 0;
      return val;
      }
}

//segitiga
float fuzzyfikasi_N(float x){
  float val;
  if(x <= -3 || x >= 3) {
    val = 0;
    return val;
    }
  else if(x >= -3 && x <= 0){
    val = (x+3)/3;
    return val;
    }
  else if(x >= 0 && x <= 3){
    val = (3-x)/3;
    return val;
    }   
}  

//linier naik

float fuzzyfikasi_EP(float x){
  float val;
  if(x <= 0){
    val = 0;
    return val;
    }
  else if(x >= 0 && x <= 3){
    val = x/3;
    return val;
    }
  else if(x >= 3){
    val = 1;
    return val;
    } 
}

//-----------------------------------------------------------------END fuzzy Input Error_Suhu---------------------------------------------------------------------------------------------


//-----------------------------------------------------------------Fuzzy Input Delta_error_Suhu-------------------------------------------------------------------------------------------

//linier turun
float fuzzyfikasi_KB(float x){
    float val;
    if(x <= -1){
      val = 1.0;
      return val;
      } 
    else if(x <= -0.5 && x >= -1 ) {
      val = (-0.5-(x))/(0.5);
      return val;
      }
    else if(x >= -0.5){
      val = 0.0;
      return val;
      }
}

// Segitiga
float fuzzyfikasi_KK(float x){
 float val;
  if(x <= -1 || x >= 0) {
    val = 0;
    return val;
    }
  else if(x >= -1 && x <= -0.5){
    val = (x+1)/0.5;
    return val;
    }
  else if(x >= -0.5 && x <= 0){
    val = (0-x)/0.5;
    return val;
    }     
}

// Segitiga
float fuzzyfikasi_Z(float x){
 float val;
  if(x <= -0.5 || x >= 0.5 ) {
    val = 0;
    return val;
    }
  else if(x >= -0.5 && x <= 0){
    val = (x+0.5)/0.5;
    return val;
    }
  else if(x >= 0 && x <= 0.5){
    val = (0.5-x)/0.5;
    return val;
    }     
}

// Segitiga
float fuzzyfikasi_PK(float x){
 float val;
  if(x <= 0 || x >= 1 ) {
    val = 0;
    return val;
    }
  else if(x >= 0 && x <= 0.5){
    val = x/0.5;
    return val;
    }
  else if(x >= 0.5 && x <= 1){
    val = (1-x)/0.5;
    return val;
    }     
}

//linier naik
float fuzzyfikasi_PB(float x){
  float val;
  if(x <= 0.5){
    val = 0;
    return val;
    }
  else if(x >= 0.5 && x <= 1){
    val = (x-0.5)/0.5;
    return val;
    }
  else if(x >= 1){
    val = 1;
    return val;
    } 
}

void fuzzyfikasi(){
  EN = fuzzyfikasi_EN(esuhu);
  N  = fuzzyfikasi_N(esuhu);
  EP  = fuzzyfikasi_EP(esuhu);
  KB  = fuzzyfikasi_KB(dsuhu);
  KK  = fuzzyfikasi_KK(dsuhu);
  Z  = fuzzyfikasi_Z(dsuhu);
  PK  = fuzzyfikasi_PK(dsuhu);
  PB  = fuzzyfikasi_PB(dsuhu);
  }
//-----------------------------------------------------------------END Fuzzy Input Delta_error_Suhu---------------------------------------------------------------------------------------------------------

//EN, N, EP, KB , KK, Z, PK,PB
void fuzzy_inferensi (){

  
//  Serial.print("Error Suhu EN : ");
//  Serial.println(EN);
//  Serial.print("Error Suhu N : ");
//  Serial.println(N);
//  Serial.print("Error Suhu EP : ");
//  Serial.println(EP);
//  Serial.print("D_Error Suhu KB : ");
//  Serial.println(KB);
//  Serial.print("D_Error Suhu KK : ");
//  Serial.println(KK);
//  Serial.print("D_Error Suhu Z : ");
//  Serial.println(Z);
//  Serial.print("D_Error Suhu PK : ");
//  Serial.println(PK);
//  Serial.print("D_Error Suhu PB : ");
//  Serial.println(PB);

// rule 1
  rule[0] = min(EN,KB);
  z_pemanas[0] = 0;
  z_kipas[0] = 255;
// rule 2
  rule[1] = min(EN,Z);
  z_pemanas[1] = 0;
  z_kipas[1] = 255;
// rule 3
  rule[2] = min(EN,KK);
  z_pemanas[2] = 0;
  z_kipas[2] = 255;
// rule 4
  rule[3] = min(EN,PB);
  z_pemanas[3] = 0;
  z_kipas[3] = 255;
// rule 5
  rule[4] = min(EN,PK);
  z_pemanas[4] = 0;
  z_kipas[4] = 255;
// rule 6
  rule[5] = min(N,KB);
  z_pemanas[5] = 0;
  z_kipas[5] = 150;
// rule 7
  rule[6] = min(N,KK);
  z_pemanas[6] = 0;
  z_kipas[6] = 150;
// rule 8
  rule[7] = min(N,Z);
  z_pemanas[7] = 0;
  z_kipas[7] = 150;
// rule 9
  rule[8] = min(N,PK);
  z_pemanas[8] = 150;
  z_kipas[8] = 150;
// rule 10
  rule[9] = min(N,PB);
  z_pemanas[9] = 150;
  z_kipas[9] = 150;
// rule 11
  rule[10] = min(EP,KB);
  z_pemanas[10] = 150;
  z_kipas[10] = 200;
// rule 12
  rule[11] = min(EP,KK);
  z_pemanas[11] = 150;
  z_kipas[11] = 200;
// rule 13
  rule[12] = min(EP,Z);
  z_pemanas[12] = 200;
  z_kipas[12] = 200;
// rule 14
  rule[13] = min(EP,PK);
  z_pemanas[13] = 200;
  z_kipas[13] = 200;
// rule 15
  rule[14] = min(EP,PB);
  z_pemanas[14] = 255;
  z_kipas[14] = 255;

  float z_total_pemanas = ((rule[0]*z_pemanas[0])+(rule[1]*z_pemanas[1])+(rule[2]*z_pemanas[2])+(rule[3]*z_pemanas[3])+(rule[4]*z_pemanas[4])+(rule[5]*z_pemanas[5])+(rule[6]*z_pemanas[6])+(rule[7]*z_pemanas[7])+(rule[8]*z_pemanas[8])+(rule[9]*z_pemanas[9])+(rule[10]*z_pemanas[10])+(rule[11]*z_pemanas[11])+(rule[12]*z_pemanas[12])+(rule[13]*z_pemanas[13])+(rule[14]*z_pemanas[14]))/(rule[0]+rule[1]+rule[2]+rule[3]+rule[4]+rule[5]+rule[6]+rule[7]+rule[8]+rule[9]+rule[10]+rule[11]+rule[12]+rule[13]+rule[13]+rule[14]);
  float z_total_kipas = ((rule[0]*z_kipas[0])+(rule[1]*z_kipas[1])+(rule[2]*z_kipas[2])+(rule[3]*z_kipas[3])+(rule[4]*z_kipas[4])+(rule[5]*z_kipas[5])+(rule[6]*z_kipas[6])+(rule[7]*z_kipas[7])+(rule[8]*z_kipas[8])+(rule[9]*z_kipas[9])+(rule[10]*z_kipas[10])+(rule[11]*z_kipas[11])+(rule[12]*z_kipas[12])+(rule[13]*z_kipas[13])+(rule[14]*z_kipas[14]))/(rule[0]+rule[1]+rule[2]+rule[3]+rule[4]+rule[5]+rule[6]+rule[7]+rule[8]+rule[9]+rule[10]+rule[11]+rule[12]+rule[13]+rule[13]+rule[14]);
  pwm_pemanas = z_total_pemanas;
  pwm_kipas = z_total_kipas;
  Serial.println(z_total_pemanas);
  Serial.println(z_total_kipas);
  
  
 }

String getStopwatchTime() {
  if (isStopwatchRunning) {
    currentTime = millis();
    elapsedTime = currentTime - startTime;

    // Hitung jam, menit, detik dari waktu yang berlalu
    unsigned long hours = elapsedTime / 3600000;
    unsigned long minutes = (elapsedTime % 3600000) / 60000;
    unsigned long seconds = ((elapsedTime % 3600000) % 60000) / 1000;

    // Buat format string untuk stopwatch
    stopwatchTime = String(hours) + ":" + String(minutes) + ":" + String(seconds);
    
    return stopwatchTime;
  } else {
    // Jika stopwatch tidak berjalan, kembalikan nilai "0:0:0"
    return stopwatchTime;
  }
}
void stopStopwatch() {
  if (isStopwatchRunning) {
    isStopwatchRunning = false;
  }
}
void startStopwatch() {
  if (!isStopwatchRunning) {
    startTime = millis();
    isStopwatchRunning = true;
  }
}
void startServer() {
  if (!isServerRunning) {
    server.begin();
    isServerRunning = true;
    Serial.println("Server started");
  }
}
void stopServer() {
  if (isServerRunning) {
    server.end();
    isServerRunning = false;
    Serial.println("Server stopped");
  }
}

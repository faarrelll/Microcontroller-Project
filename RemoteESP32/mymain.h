// mymain.h

#ifndef MYMAIN_H
#define MYMAIN_H

#include <WebServer.h>
#include "DHT.h"
#include <Wire.h>
#include <RTClib.h>
#include <EEPROM.h>

extern WebServer server; // Declare the server object as external
RTC_DS3231 rtc;


bool isLogin = false;
float temperature = 0;

String pumpTime = ""; // Variabel global untuk menyimpan waktu pompa yang telah diatur
String pumpTimeOff = "";
String Datetime = "";
//const int relay1 = 16; //PLN FASA
const int relay1 = 17; //PLN
const int relay2 = 18; // inverter
//const int relay4 = 19; // ON OFF INTERTER
const int voltage_sensor = 4;
const int voltage_bat = 33;
int eepromAddressPumpTime = 10;
int eepromAddressPumpTimeOff = 60;
bool isDelay = true;

long lastMsg = 0;
long lastMsg2 = 0;


String readStringFromEEPROM(int startAddress) {
  int stringLength = EEPROM.read(startAddress);
  char data[stringLength + 1];

  for (int i = 0; i < stringLength; i++) {
    data[i] = EEPROM.read(startAddress + 1 + i);
  }
  data[stringLength] = '\0'; // Tambahkan null terminator untuk mengubahnya menjadi string

  return String(data);
}

void saveStringToEEPROM(int startAddress, const String &data) {
  int stringLength = data.length();
  EEPROM.write(startAddress, stringLength);

  for (int i = 0; i < stringLength; i++) {
    EEPROM.write(startAddress + 1 + i, data[i]);
  }
  EEPROM.commit(); // Pastikan data disimpan pada EEPROM (untuk ESP8266/ESP32)
}

int parseTimeString(const String& timeString) {
  int separatorIndex = timeString.indexOf(':');
  if (separatorIndex == -1) return -1; // Jika format waktu tidak valid, kembalikan -1
  int hour = timeString.substring(0, separatorIndex).toInt();
  int minute = timeString.substring(separatorIndex + 1).toInt();
  return hour * 100 + minute;
}
void MySetup() {
//  SetupRoutes();
//  Serial.begin(115200);
   rtc.begin();
//  if (!rtc.begin()) {
//    Serial.println("Couldn't find RTC");
//      while (1);
//  }
//    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

//  if (rtc.lostPower()) {
////    Serial.println("RTC lost power, setting the time!");
//    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
//  }

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
//  pinMode(relay3, OUTPUT);
//  pinMode(relay4, OUTPUT);
  pinMode(voltage_sensor,INPUT_PULLUP);
  pinMode(2,OUTPUT);
  pumpTime = readStringFromEEPROM(eepromAddressPumpTime);
  pumpTimeOff = readStringFromEEPROM(eepromAddressPumpTimeOff);
 
  
  
  server.on("/setPumpTime", HTTP_POST, []() {
  if (server.hasArg("pumpTime")) {
    pumpTime = server.arg("pumpTime");
    saveStringToEEPROM(eepromAddressPumpTime, pumpTime);
    server.send(200, "text/plain", "Pump time set to: " + pumpTime);
  } else {
    server.send(400, "text/plain", "Invalid request");
  }
});
  server.on("/setPumpTimeOff", HTTP_POST, []() {
  if (server.hasArg("pumpTimeOff")) {
    pumpTimeOff = server.arg("pumpTimeOff");
    saveStringToEEPROM(eepromAddressPumpTimeOff, pumpTimeOff);
    server.send(200, "text/plain", "Pump time off set to: " + pumpTimeOff);
  } else {
    server.send(400, "text/plain", "Invalid request");
  }
});
  server.on("/time", HTTP_GET, []() {
    DateTime now = rtc.now();
    String currentTime = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day()) + " " + String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
    String json = "{\"time\": \"" + currentTime + "\"}";
    server.send(200, "application/json", json);
  });
  server.on("/setDate", HTTP_POST, []() {
  if (server.hasArg("dateTime")) {
    Datetime = server.arg("dateTime");
    server.send(200, "text/plain", "Date time set to: " + Datetime);
    int year = Datetime.substring(0, 4).toInt();
    int month = Datetime.substring(5, 7).toInt();
    int day = Datetime.substring(8, 10).toInt();
    int hour = Datetime.substring(11, 13).toInt();
    int minute = Datetime.substring(14, 16).toInt();
    int second = 0; // Assuming seconds are not provided in the input

    // Adjust the RTC with the parsed datetime
    rtc.adjust(DateTime(year, month, day, hour, minute, second));
//    Serial.println(Datetime);
  } else {
    server.send(400, "text/plain", "Invalid request");
  }
});
  server.on("/voltage", HTTP_GET, []() {
    int value_sensor = digitalRead(voltage_sensor);
    String value;
    if(value_sensor == LOW){
      value = "ON";
    } else {
      value = "OFF";
    }
     String json = "{\"voltage\":\"" + value + "\"}";
     server.send(200, "application/json", json);
  });
  server.on("/resetCount", HTTP_GET, []() {
    int  rst = EEPROM.read(0);
     String val_rst = String(rst);
//     String tes = "tes";
     String json = "{\"resetCount\":\"" + val_rst + "\"}";
     server.send(200, "application/json", json);
  });
  server.on("/voltageBat", HTTP_GET, []() {
    int value_bat = analogRead(voltage_bat);
    float volt = (value_bat+219.88) / 254.96 ;
    // int volt = map(value_bat, 0,3098,0,13);
    String valuebat = String(volt);
     String json = "{\"voltageBat\":\"" + valuebat + "\"}";
     server.send(200, "application/json", json);
  });

}

//void flashLed(int LedPin, unsigned long timeflash) {
//  if (currentMillis - ledtime >= timeflash) {
//    ledstate = !ledstate;
//    digitalWrite(LedPin, ledstate);
//    ledtime = currentMillis;
//  }
//}

void MyLoop() {
//  delay(1);
  DateTime now = rtc.now();
  // String currentTime = String(now.hour()) + ":" + String(now.minute());
  int currentHour = now.hour();
  int currentMinute = now.minute();
  int currentTime = currentHour * 100 + currentMinute; 
  int pumpOnTimeInt = parseTimeString(pumpTime); // Ubah string waktu menjadi integer
  int pumpOffTimeInt = parseTimeString(pumpTimeOff);
  int val_bat = analogRead(voltage_bat);
  float tegangan = (val_bat+219.88) / 254.96 ;
  // float hasil_tegangan = 0;
  
  // for(int x = 1; x<=50; x++){
  //   int val_bat = analogRead(voltage_bat);
  //   float tegangan = (12.8*val_bat) / 2815 ;
  //   hasil_tegangan += tegangan;
  //   // Serial.println(x);
  //   }
    
    // hasil_tegangan = hasil_tegangan/50;
    //ponpes

      
    if (currentTime >= pumpOnTimeInt && currentTime < pumpOffTimeInt ) {

      digitalWrite(relay1, HIGH); // PLN
      // digitalWrite(relay2, HIGH); // Inverter 
      long now = millis();
      if (now - lastMsg > 5000) {
        digitalWrite(relay2, HIGH); // Inverter 
        lastMsg = now;
      }
      lastMsg2= now;

  } else{
      
      digitalWrite(relay2, LOW);  
      // digitalWrite(relay1, LOW); 
      long now = millis();
      if (now - lastMsg2 > 5000) {
        lastMsg2 = now;
        digitalWrite(relay1, LOW); 
        
 
      }
            lastMsg= now;

  }
      

  


}

//const int relay1 = 17; //PLN 
//const int relay2 = 18; //inverter



#endif

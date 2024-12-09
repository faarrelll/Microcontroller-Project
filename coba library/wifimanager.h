#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H
#include <Arduino.h>
#include <WiFi.h>
class wifiku{
  private:
    String ssid;
    String password;
    
  public:

  wifiku(){}
  wifiku(String ssid, String pass){
      this->ssid = ssid;
      password = pass;
    }
  
    void init_wifi(){
    WiFi.begin(ssid.c_str(), password.c_str());
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(300);
    }
    }
  };

#endif

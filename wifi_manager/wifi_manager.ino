#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager


void setup() {
    Serial.begin(115200);
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
    }

}

void loop() {

}

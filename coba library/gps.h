#ifndef GPS_H
#define GPS_H
#include <Arduino.h>
#include <TinyGPSPlus.h>
TinyGPSPlus gps;

class gps_mobil{

  private:
  int a;

  public:

  gps_mobil(){}
  gps_mobil(int a){
    Serial.println(a);

  }
  void get_data_gps(){

   while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
      displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }

  }
  void displayInfo()
  {

    Serial.print(F("Location: "));
    if (gps.location.isValid()){
      Serial.print("Lat: ");
      Serial.print(gps.location.lat(), 6);
      Serial.print(F(","));
      Serial.print("Lng: ");
      Serial.print(gps.location.lng(), 6);
      Serial.println();
    }
    else {
      Serial.print(F("INVALID"));
    }
  }
  


};


#endif
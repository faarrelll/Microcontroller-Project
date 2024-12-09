#include <ArduinoJson.h>
#include <HardwareSerial.h>

//#define tx 17
//#define rx 16
HardwareSerial mySerial(Serial2); // RX, TX

JsonDocument doc;

String output;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mySerial.begin(115200, SERIAL_8N1, 16, 17);


}

void loop() {
  // put your main code here, to run repeatedly:
  doc["kt1"] = 1;
  doc["kt2"] = 2;
  doc["kt3"] = 3;
  doc["ka1"] = 1;
  doc["ka2"] = 2;
  doc["ka3"] = 3;

  serializeJson(doc, output);

//  Serial.println(output);
  mySerial.println(output);
//  mySerial.println(output+"#######");
  delay(1000);

}

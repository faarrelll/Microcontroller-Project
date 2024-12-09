#include <ArduinoJson.h>
#include <HardwareSerial.h>

//#define tx 17
//#define rx 16

HardwareSerial mySerial(Serial2); // RX, TX
JsonDocument doc;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
//  Serial2.begin(115200,rx,tx);
  mySerial.begin(115200, SERIAL_8N1, 16, 17);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (mySerial.available() > 0) {

    String cekwoy= mySerial.readString();
//    String cek = mySerial.readStringUntil('#######');
    Serial.println(cekwoy);
//    deserializeJson(doc, cek);
//    Serial.println(doc);
//    JsonObject obj = doc.as<JsonObject>();
//    Serial.println(obj);
    
//    int kt1 = doc["kt1"]; // 1
//    int kt2 = doc["kt2"]; // 2
//    int kt3 = doc["kt3"]; // 3
//    int ka1 = doc["ka1"]; // 1
//    int ka2 = doc["ka2"]; // 2
//    int ka3 = doc["ka3"]; // 3
//    Serial.print("kt 1: ");
//    Serial.println(kt1);
//    Serial.print("kt 2: ");
//    Serial.println(kt2);
//    Serial.print("kt 3: ");
//    Serial.println(kt3);
//    Serial.print("ka 1: ");
//    Serial.println(ka1);
//    Serial.print("ka 2: ");
//    Serial.println(ka2);
//    Serial.print("ka 3: ");
//    Serial.println(ka3);
  }
}

#include <HardwareSerial.h>
HardwareSerial mySerial(Serial2); // RX, TX

void setup() {
  Serial.begin(115200);
  Serial.println("Starting System");
  mySerial.begin(9600);

}

void loop() {
  mySerial.print("Req%");
  Serial.println("Data yang dikirim ESP = Req\n");
  unsigned long timeLimit = millis();
  Serial.println("waiting data\n");
  while (millis() - timeLimit < 3000) {
    if (mySerial.available()) {
      String dataIN = "";
      dataIN = mySerial.readStringUntil('%');
      Serial.println(dataIN);
      break;
    }
  }
  Serial.println("waiting loop");
  Serial.println();
  delay(4000);
}

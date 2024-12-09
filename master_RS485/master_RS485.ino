#include <HardwareSerial.h>
#define enTxPin 5   // HIGH: TX and LOW: RX 
HardwareSerial mySerial(Serial2); // RX, TX

void setup() {
  Serial.begin(115200);
  Serial.println("Starting System");

  mySerial.begin(9600);

  pinMode(enTxPin, OUTPUT);
  digitalWrite(enTxPin, HIGH);       // default TX
}

void loop() {
  mySerial.print("Req%");
  Serial.println("Data yang dikirim = Req");
  digitalWrite(enTxPin, LOW);       // change pin to RX
  unsigned long timeLimit = millis();
  Serial.println("waiting data");
  while (millis() - timeLimit < 3000) {
    if (mySerial.available()) {
      String dataIN = "";
      dataIN = mySerial.readStringUntil('%');
      Serial.print("Data yang diterima = ");
      Serial.println(dataIN);
      digitalWrite(enTxPin, HIGH);       // default TX
      break;
    }
  }
  Serial.println("waiting loop");
  Serial.println();
  delay(1000);
}

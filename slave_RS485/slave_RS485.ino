#include <HardwareSerial.h>
#define enTxPin 5   // HIGH: TX and LOW: RX 
HardwareSerial mySerial(Serial2); // RX, TX

void setup() {
  Serial.begin(115200);
  Serial.println("Starting System");

  mySerial.begin(9600);

  pinMode(enTxPin, OUTPUT);
  digitalWrite(enTxPin, LOW);       // default RX
}

void loop() {
  if (mySerial.available()) {
    String dataIN = "";

    dataIN = mySerial.readStringUntil('%');
    Serial.print("Data yang diterima = ");
    Serial.println(dataIN);

    if (dataIN == "Req"){
      Serial.println("kirim data ke master");
      digitalWrite(enTxPin, HIGH);       // change pin to HIGH for TX
      String dataOUT = "Respon";
      Serial.println("Data yang dikirim = Respon\n\n");
      mySerial.print(dataOUT);
      digitalWrite(enTxPin, LOW);       // default RX
    }
  }

  delay(10);
}

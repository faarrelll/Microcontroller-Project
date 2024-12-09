void setup() {
  Serial.begin(9600); 
}

void loop() {
  if (Serial.available()) {
    String dataIN = "";
    dataIN = Serial.readStringUntil('%');
    Serial.print("Data yang diterima Arduino = ");
    Serial.println(dataIN);
    if (dataIN == "Req"){
      Serial.println("Data yang dikirim Arduino dan diterima ESP = Respon\n");
    }
  }                       
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Mulai oy");
}

void loop() {
  // put your main code here, to run repeatedly:
  // untuk inputan string
  if(Serial.available() > 0 ){
    String s = Serial.readString();
    Serial.println(s);
    }

  // untuk inputan char
   if(Serial.available() > 0 ){
    String s = Serial.readString();
    Serial.println(s);
    }
}

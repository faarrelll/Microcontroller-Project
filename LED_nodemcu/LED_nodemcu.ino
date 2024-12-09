#define pinLED D5

void setup() {
  // put your setup code here, to run once:
  pinMode(pinLED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(pinLED, HIGH);
  delay(100);
  digitalWrite(pinLED, LOW);
  delay(100);
  
}

#define led D5
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(led,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(led,HIGH);
  delay(100);
  digitalWrite(led,LOW);
  delay(100);

  Serial.println("Bismillah");
}

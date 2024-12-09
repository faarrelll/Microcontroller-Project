#define ledr 9
#define ledb 8
#define ledg 10
void setup() {
  // put your setup code here, to run once:
  pinMode(ledr, OUTPUT);
  pinMode(ledg, OUTPUT);
  pinMode(ledb, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ledg, HIGH);
  delay(1000);
  digitalWrite(ledg,LOW);
  delay(1000);
  digitalWrite(ledb, HIGH);
  delay(1000);
  digitalWrite(ledb,LOW);
  delay(1000);
  digitalWrite(ledr, HIGH);
  delay(1000);
  digitalWrite(ledr,LOW);
  delay(1000);
}

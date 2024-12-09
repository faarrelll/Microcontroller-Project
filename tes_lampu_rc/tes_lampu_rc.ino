#define lamp_d 25
#define lamp_b 27
#define riting_kn 32
#define riting_kr 33

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(lamp_d,OUTPUT);
  pinMode(lamp_b,OUTPUT);
  pinMode(riting_kn,OUTPUT);
  pinMode(riting_kr,OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(lamp_d, HIGH);
  delay(3000);
  digitalWrite(lamp_d, LOW);
  digitalWrite(lamp_b, HIGH);
  delay(3000);
  digitalWrite(lamp_b, LOW);
  digitalWrite(riting_kn, HIGH);
  delay(1000);
  digitalWrite(riting_kn, LOW);
  delay(500);
  digitalWrite(riting_kn, HIGH);
  delay(500);
  digitalWrite(riting_kn, LOW);
  delay(500);
  digitalWrite(riting_kr, HIGH);
  delay(500);
  digitalWrite(riting_kr, LOW);
  delay(500);
  digitalWrite(riting_kr, HIGH);
  delay(500);
  digitalWrite(riting_kr, LOW);
  delay(500);

}

int sudutrad;
float hasil;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sudutrad=0;
}

void loop() {
  // put your main code here, to run repeatedly:
  ++sudutrad;
  hasil = sin(sudutrad*3.14/180);
  Serial.println(hasil);
  delay(10);
}

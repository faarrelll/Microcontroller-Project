#define solar_sensor A0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(solar_sensor, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int val = analogRead(solar_sensor);
  Serial.print("analog : ");
  Serial.println(val);
   Serial.print("tegangan : ");
  float val2 = val*(5.0/1023.0);
  Serial.println(val2);
  Serial.print("Solar W/m2 : ");
  float val3 = (1800.0*val2)/5.0;
  Serial.println(val3);
  delay(1000);
}

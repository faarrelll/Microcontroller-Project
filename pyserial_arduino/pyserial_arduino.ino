char dariPython;
int pin = 8;
 
void setup() {
  pinMode(pin, OUTPUT);
  Serial.begin(9600);
}
 
void loop() {
  if(Serial.available() > 0)
  {
    dariPython = Serial.read();
    Serial.println(dariPython);
  }
 
  if(dariPython == '1')
  {
    digitalWrite(pin, HIGH);
  }
  else
  {
    digitalWrite(pin, LOW);
  }
}

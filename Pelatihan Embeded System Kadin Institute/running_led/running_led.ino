const int ledpins[] = {5,18,19};
const int numled = 3;

void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i < numled; i++){
    pinMode(ledpins[i], OUTPUT);
  }
}

void loop() {
  for(int a = 0; a<1; a++){
  for(int i = 0; i < numled; i++){
    for(int j = 0; j < numled; j++){
      digitalWrite(ledpins[j],LOW);
    }
    digitalWrite(ledpins[i],HIGH);
    delay(500);
  }
  digitalWrite(ledpins[2],LOW);
  digitalWrite(ledpins[1],HIGH);
  delay(500);
  }
 
  }


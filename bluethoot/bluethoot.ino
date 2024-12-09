int led=13;
int DataBluetooth;

void setup() {
  Serial.begin(9600);
  Serial.println("Menyalakan dan mematikan led dengan bluetooth");
  Serial.println("1 : ON, 0 = Off");
  pinMode(led,OUTPUT);
}

void loop() {
  if (Serial.available()){
   DataBluetooth=Serial.read();
   if(DataBluetooth=='1'){  
     digitalWrite(led,HIGH);
     Serial.println("LED On");
   }
   if (DataBluetooth=='0'){
      digitalWrite(led,LOW);
      Serial.println("LED Off");
  }
}
delay(100);
}

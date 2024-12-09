#define triggerPin  D5
#define echoPin     D6
#define buz         D8
//#define mode        D7
#define water A0


void setup() {
  Serial.begin (9600);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buz,OUTPUT);
  pinMode(water,INPUT);
//  pinMode(mode,INPUT_PULLUP);
}
void loop() {

//  air();
//  int modenih = digitalRead(mode);
//  if(modenih == 0){
    hcsr();
//    Serial.println("gas");
//    }
//  else{
//    Serial.println("hop");
//    }
  
  

//  Serial.flush();
}

void hcsr(){
  int air = analogRead(water);
  Serial.println(air);
  
  long duration, jarak;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2); 
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  jarak = (duration/2) / 29.1;
  Serial.println("jarak :");
  Serial.print(jarak);
  Serial.println(" cm");
  if(jarak < 30) {
     digitalWrite(buz, HIGH);
     delay(20);
     digitalWrite(buz, LOW);
     delay(50);
//     Serial.println("awas objek terlalu dekat");
   }
   else if(air>350) {
        digitalWrite(buz,HIGH);
        } else {
          digitalWrite(buz,LOW);
          }
  
    
  }
  

void air(){
  
  int air = analogRead(water);
  Serial.println(air);
    if(air>350){
      digitalWrite(buz,HIGH);
      Serial.println("genangan air terdeteksi");
      } else if(air>350) {
        digitalWrite(buz,HIGH);
        } else {
          digitalWrite(buz,LOW);
          }

      
  
  }

#define echoPin 2 
#define trigPin 3 
#define relay 4
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);


long duration; 
float distance; 
int addr = 0;
int addr2 = 1;
int val = 100;
int val2;

void setup() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(relay,OUTPUT);
  Serial.begin(9600);
  lcd.begin(); 
  EEPROM.write(addr,val);
  val = EEPROM.read(addr);
  Serial.print("nilai sebelumnya : ");
  Serial.println(val);
  digitalWrite(relay,HIGH);
}
void loop() {
  // Clears the trigPin condition
  val2 = EEPROM.read(addr2);
  if(val2 == 1){
    
    Serial.println("pompa nyala");
    digitalWrite(relay,LOW);
    pompa();
    
    
    }
  
  else{
    
    sensor();
    
  
  }
  
  
}

void sensor(){
 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  
  if(distance > 13){
    distance = 13;
    }
  else if(distance < 5){
    distance = 5;
    }
  val = distance;
  val = map(val,5,13,100,0);
  if (val <= 20){
    EEPROM.write(addr2,1);
    }
  EEPROM.write(addr,val);
  Serial.println(val);
  layar();
  delay(500); 
  
}

void pompa(){

  sensor();
  if(val == 100){
    
    EEPROM.write(addr2,0);
    digitalWrite(relay,HIGH);
    Serial.println("Pompa mati");
    
    }
  
  }
void layar(){
//  lcd.clear();
  String tampil_lcd;
  if(val < 100 && val > 10){
    tampil_lcd = "0" + String(val);
    }
  else if(val < 10){
    tampil_lcd = "00" + String(val);
    }
  else if (val == 100){
    tampil_lcd = String(val);
    }
    
  lcd.setCursor(3,0);
  lcd.print("Kelompok 5");
  lcd.setCursor(0,1);
  lcd.print("Isi Air = ");
//  lcd.setCursor(11,1);
  lcd.print(tampil_lcd);
  lcd.setCursor(14,1);
  lcd.print("%");
//  Serial.flush(); 
 }

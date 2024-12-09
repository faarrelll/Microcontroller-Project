#include <Servo.h> // menyertakan library servo ke dalam program 
Servo myservo;     // variable untuk menyimpan posisi data
int pos = 00;   
int tunda=100;
         
void setup(){ 
 myservo.attach(9); //sinyal data kabel motor servo dikonekan di pin 3 Arduino
 Serial.begin(9600);
} 
 
void loop(){ 
 for(pos = 00; pos < 180; pos += 1)  //fungsi perulangan yang akan dijadikan PWM dengan kenaikan 1
 {
  tunda=(int)analogRead(A5)/4;
  tunda=256-tunda;
  Serial.println(tunda);
  myservo.write(pos); //prosedur penulisan data PWM ke motor servo
  delay(tunda); //waktu tunda 15 ms                 
 } 
 for(pos = 180; pos>=1; pos-=1)  //fungsi perulangan perubahan PWM dg penurunan 1
 {                              
  tunda=(int)analogRead(A5)/4;
  tunda=256-tunda;
  myservo.write(pos);                 
  delay(tunda);                        
 }
}

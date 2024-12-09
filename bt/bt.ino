#include <SoftwareSerial.h> 
SoftwareSerial module_bluetooth(0, 1); // pin RX | TX
 
char data = 0;             
void setup() 
{
  Serial.begin(9600);         
  pinMode(13, OUTPUT);  //inisialisasi LED menjadi output
}
void loop()
{
  if(Serial.available() > 0)  
  {
    data = Serial.read();Serial.print(data);    //Pembacaan dan ditampilkan data yang masuk   
    Serial.print("\n"); 
    //Data yang masuk      
    if(data == '1'){
    digitalWrite(13, HIGH);//diberikan waktu tunda 1 detik
    }
    else if(data == '0')      
    digitalWrite(13, LOW);  
  }                            
 
}
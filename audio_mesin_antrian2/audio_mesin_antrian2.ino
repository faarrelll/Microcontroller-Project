#include <esp_now.h>
#include <WiFi.h>
#include <Arduino.h>

#include "SPIFFS.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"
#define mybutton 18

AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2SNoDAC *out;
AudioFileSourceID3 *id3;

int counter= 5;
int no_antrian;
int trigger = 0;

typedef struct struct_message{

 int ngeprint;
 int antrian;
 
  
  } struct_message;


struct_message myData;

void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  (void)cbData;
  Serial.printf("ID3 callback for: %s = '", type);

  if (isUnicode) {
    string += 2;
  }
  
  while (*string) {
    char a = *(string++);
    if (isUnicode) {
      string++;
    }
    Serial.printf("%c", a);
  }
  Serial.printf("'\n");
  Serial.flush();
}

void mulai_musik(const char *lagunya) {
  audioLogger = &Serial;
  file = new AudioFileSourceSPIFFS(lagunya);
  id3 = new AudioFileSourceID3(file);
  id3->RegisterMetadataCB(MDCallback, (void *)"ID3TAG");
  out = new AudioOutputI2SNoDAC();
  mp3 = new AudioGeneratorMP3();
  mp3->begin(id3, out);
    while(mp3->isRunning()) {
    if (!mp3->loop()) mp3->stop();
  } 

}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len){
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Byte received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.antrian);
  no_antrian = myData.antrian;
  trigger = 1;
  
  }
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
   
  if(esp_now_init() != ESP_OK){
    Serial.println("Error initializing ESP-NOW");
    return;
    }

  esp_now_register_recv_cb(OnDataRecv);
  SPIFFS.begin();
  pinMode(mybutton, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(trigger == 1){
      baca(no_antrian);
      trigger = 0;
    } 
}

void sortir(){
  
  if (counter >= 0){
  if(no_antrian == 1 ){
      counter += 1;
      Serial.println(counter);
    } else if(no_antrian == 2){
      counter = counter - 1;
      no_antrian = 0;
      if(counter == 0){
        counter = 0;
        no_antrian = 0;
        }
      }
  }
  Serial.println(counter);
  baca(counter);
  }
  
void baca(int angka){
  if(angka>99){
    mulai_musik("/nomorantrian.mp3");
    ratusan(angka);
    mulai_musik("/sumber_suara.mp3"); 
  }
  else if(angka>9){
    mulai_musik("/nomorantrian.mp3");
    puluhan(angka);
    mulai_musik("/sumber_suara.mp3");  
  }
  else if(angka>0){
    mulai_musik("/nomorantrian.mp3");
    satuan(angka);
    mulai_musik("/sumber_suara.mp3");
  }
}

void puluhan(int angka){
  
  int tmp1,tmp2;
  tmp1=angka/10;
  tmp2=angka%10;
  
  switch(tmp1){
   case 1:{
    if(tmp2==0)        
    {
      
      mulai_musik("/se.mp3");
      mulai_musik("/puluh.mp3");
      Serial.print("Sepuluh ");
    }
    else if(tmp2==1){

      mulai_musik("/se.mp3");
      mulai_musik("/belas.mp3");
      Serial.print("Sebelas "); 
      }   
    else if(tmp2==2) {  

      mulai_musik("/dua.mp3");
      mulai_musik("/belas.mp3");
      Serial.print("Dua Belas ");
    }
    else if(tmp2==3){     

      mulai_musik("/tiga.mp3");
      mulai_musik("/belas.mp3");
      Serial.print("Tiga Belas ");
    }
    else if(tmp2==4) {   

      mulai_musik("/empat.mp3");
      mulai_musik("/belas.mp3");
      Serial.print("Empat Belas "); }
    else if(tmp2==5) {  

      mulai_musik("/lima.mp3");
      mulai_musik("/belas.mp3");
      Serial.print("Lima Belas "); }
    else if(tmp2==6)  { 
      
      mulai_musik("/enam.mp3");
      mulai_musik("/belas.mp3");
      Serial.print("Enam belas ");}
    else if(tmp2==7)  { 

      mulai_musik("/tujuh.mp3");
      mulai_musik("/belas.mp3");
      Serial.print("Tujuh Belas ");}
    else if(tmp2==8) {  
   
      mulai_musik("/delapan.mp3");
      mulai_musik("/belas.mp3");
      Serial.print("Delapan Belas "); }
    else if(tmp2==9){

      mulai_musik("/sembilan.mp3");
      mulai_musik("/belas.mp3");
      Serial.print("Sembilan Belas "); }
    break;
   } 
    case 2: {
   
      mulai_musik("/dua.mp3");
      mulai_musik("/puluh.mp3");
      Serial.print("Dua Puluh ");break; }
    case 3: {

      mulai_musik("/tiga.mp3");
      mulai_musik("/puluh.mp3");
      Serial.print("Tiga Puluh ");break; }
    case 4: {
    
      mulai_musik("/empat.mp3");
      mulai_musik("/puluh.mp3");
      Serial.print("Empat Puluh ");break;}
    case 5: {
 
      mulai_musik("/lima.mp3");
      mulai_musik("/puluh.mp3");
      Serial.print("Lima Puluh ");break;}
    case 6:{

      mulai_musik("/enam.mp3");
      mulai_musik("/puluh.mp3");
      Serial.print("Enam Puluh ");break;}
    case 7:{
 
      mulai_musik("/tiga.mp3");
      mulai_musik("/puluh.mp3");
      Serial.print("Tujuh Puluh ");break;}
    case 8:{
      mulai_musik("/delapan.mp3");
      mulai_musik("/puluh.mp3");
      Serial.print("Delapan Puluh ");break;}
    case 9: {

      mulai_musik("/sembilan.mp3");
      mulai_musik("/puluh.mp3");
      Serial.print("Sembilan Puluh ");break;}
  }
//  delay(1300);
  if(tmp2>0&&angka>19)satuan(tmp2);
}

void satuan(int angka){
  
 switch(angka){
  case 1: {             
    mulai_musik("/satu.mp3");
    Serial.print("Satu ");break; }
  case 2: {             
    mulai_musik("/dua.mp3");
    Serial.print("Dua ");break; }
  case 3: {              
    mulai_musik("/tiga.mp3");
    Serial.print("Tiga ");break;
  }
  case 4: {           
    mulai_musik("/empat.mp3");
    Serial.print("Empat ");break;
  }
  case 5: {             
    mulai_musik("/lima.mp3");
    Serial.print("Lima ");break;
  }
  case 6: {          
    
    mulai_musik("/enam.mp3");
    
    Serial.print("Enam ");break;
  }
  case 7: {             
    mulai_musik("/tujuh.mp3");
    Serial.print("Tujuh ");break;
  }
  case 8: {            
    mulai_musik("/delapan.mp3");
    Serial.print("Delapan ");break;
  }
  case 9: {             
    mulai_musik("/sembilan.mp3");
    Serial.print("Sembilan ");break;
  }
 } 
// delay(700);
}


void ratusan(int angka){
  int tmp1,tmp2;
  tmp1=angka/100; tmp2=angka%100;
  //Serial.println();
  switch(tmp1){
case 1: { 
  mulai_musik("/se.mp3");
  mulai_musik("/ratus.mp3");
  Serial.print("Seratus ");break;}
case 2:{ 
  mulai_musik("/dua.mp3");
  mulai_musik("/ratus.mp3");
  Serial.print("Dua Ratus ");break;
}
case 3: {
  mulai_musik("/tiga.mp3");
  mulai_musik("/ratus.mp3");
  Serial.print("Tiga Ratus ");break;
}
case 4: { 
  mulai_musik("/empat.mp3");
  mulai_musik("/ratus.mp3");
  Serial.print("Empat Ratus ");break;
}
case 5: {
  mulai_musik("/lima.mp3");
  mulai_musik("/ratus.mp3");
  Serial.print("Lima Ratus ");break;
}
case 6: {
  mulai_musik("/enam.mp3");
  mulai_musik("/ratus.mp3");
  Serial.print("Enam Ratus ");break;
}
case 7: 
{
  mulai_musik("/tujuh.mp3");
  mulai_musik("/ratus.mp3");
  Serial.print("Tujuh Ratus ");break;
}
case 8: {
  mulai_musik("/delapan.mp3");
  mulai_musik("/ratus.mp3");
  Serial.print("Delapan Ratus ");break;
}
case 9: {
  mulai_musik("/sembilan.mp3");
  mulai_musik("/ratus.mp3");
  Serial.print("Sembilan Ratus ");break;
}
  }
  //Serial.println(tmp1);
  delay(1000);
  if(tmp2>9)puluhan(tmp2);
  else if(tmp2>0)satuan(tmp2);
}

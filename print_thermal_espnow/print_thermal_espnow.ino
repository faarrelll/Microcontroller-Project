#include "Adafruit_Thermal.h"
#define buttonA 5
#include <esp_now.h>
#include <WiFi.h>
#include <EEPROM.h>
HardwareSerial mySerial(Serial2);
Adafruit_Thermal printer(&mySerial);
int antrian_A = 6;
int antrian_B = 0;
int antrian_C = 6; 
String val_A;
String val_B;
String val_C;
int addr = 0;
int addr2 = 0;


//uint8_t broadcastAddress[] =  {0x84, 0x0D, 0x8E, 0xE9, 0x5E, 0x58};
uint8_t broadcastAddress[] =  {0x7C, 0x9E, 0xBD, 0xE3, 0x4AE, 0x0C };
//08:3A:F2:AC:66:64
//8C:AA:B5:86:45:8C



typedef struct struct_message{
  
  int ngeprint;
  int antrian;
  
  } struct_message;

typedef struct struct_message2{
  
  int batasmax;
  
  } struct_message2;

struct_message myData;

struct_message2 batas;

esp_now_peer_info_t peerInfo;

void OnDataSent (const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  }
  

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len){
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Byte received: ");
  Serial.println(len);
  Serial.print("Int: ");
//  Serial.println(myData.antrian);
//  val_B = myData.antrian;
  antrian_B = myData.ngeprint;
  }

void setup() {
  Serial.begin(115200);
  EEPROM.begin(64);
  WiFi.mode(WIFI_STA);
  pinMode(buttonA, INPUT_PULLUP);
  antrian_A = EEPROM.read(addr);
  if(antrian_A == 255){
    antrian_A = 6;
    }
  antrian_C = EEPROM.read(addr2);
  if(antrian_C == 6){
    antrian_C = 6;
    }
  if(esp_now_init() != ESP_OK){
    Serial.println("Error initializing ESP-NOW");
    return;
    }
  
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_register_send_cb(OnDataSent);

  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
   if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  mySerial.begin(115200);
  printer.begin();
//  val_epprom = EEPROM.read(addr);
 

}
void loop() {
 int btn_A = digitalRead(buttonA);
 sortir_karcis();
// Serial.println(btn_A);
 if(btn_A == 0){
    loker_A();
  }
 if(antrian_B == 1){
  loker_B(); // poli umum
  } else if (antrian_B == 2){
    loker_C(); //poli gigi
    } else if (antrian_B == 3){
    loker_D(); // Poli KIA
    } else if (antrian_B == 4){
    loker_E(); // Poli Infeksius
    } else if (antrian_B == 5){
    loker_F(); // Poli TB
    } else if(antrian_B == 10){
      antrian_A = 6;
      EEPROM.write(addr,antrian_A);
      EEPROM.commit();
      antrian_C = 6;
      EEPROM.write(addr,antrian_C);
      EEPROM.commit();
      batas.batasmax = 5;
      esp_err_t result = esp_now_send(0, (uint8_t *) &batas, sizeof(batas));
      antrian_B = 0; 
      
      } 
}

void sortir_karcis(){

  if (antrian_A < 100 && antrian_A < 10){
    val_A = "00" + String(antrian_A);
  }
 else if (antrian_A >= 10 && antrian_A < 100) {
    val_A = "0" + String(antrian_A);
  } else {
    val_A =  String(antrian_A);
    }

  if (antrian_C < 100 && antrian_C < 10){
    val_C = "00" + String(antrian_C);
  }
 else if (antrian_C >= 10 && antrian_C < 100) {
    val_C = "0" + String(antrian_C);
  } else {
    val_C =  String(antrian_C);
    }
 
  }

void loker_A(){

  batas.batasmax = antrian_A;
  esp_err_t result = esp_now_send(0, (uint8_t *) &batas, sizeof(batas));
  printer.justify('C');
  printer.boldOn();
  printer.setSize('M');
  printer.println("Puskesmas Kedungkandang \n");
//  printer.setSize('S');
//  printer.boldOff();
//  printer.println("Jl. Raya Ki Ageng Gribig No.142, Kedungkandang, Kota Malang\n");
  printer.setSize('M');
  printer.println("Loket Pendaftaran \n");
//  printer.boldOn();
  printer.setSize('L');
  printer.println(val_A);
  printer.println("");
  
  
  printer.setSize('S');
  printer.println("Terima kasih sudah mengantri \n\n\n"); 
//  printer.boldOff();
//  antrian = antrian + 1;
  printer.wake();
  printer.setDefault();
  antrian_A = antrian_A + 1;
  EEPROM.write(addr,antrian_A);
  EEPROM.commit();
  
  
  }

void loker_B(){

  printer.justify('C');
  
  printer.boldOn();
  printer.setSize('M');
  printer.println("Puskesmas Kedungkandang \n");
//  printer.setSize('S');
//  printer.boldOff();
//  printer.println("Jl. Raya Ki Ageng Gribig No.142, Kedungkandang, Kota Malang\n");
  printer.setSize('M');
  printer.println("Poli Umum \n");
//  printer.boldOn();
  printer.setSize('L');
  printer.println(val_C);
  printer.println("");
  
  
  
  printer.setSize('S');
  printer.println("Terima kasih sudah mengantri \n\n\n"); 
//  printer.boldOff();
//  antrian = antrian + 1;
  printer.wake();
  printer.setDefault();
  antrian_B = 0;
  antrian_C +=1;
  EEPROM.write(addr2,antrian_C);
  EEPROM.commit();
//  EEPROM.write(addr,antrian_C); 
  }

void loker_C(){

  printer.justify('C');
  
  printer.boldOn();
  printer.setSize('M');
  printer.println("Puskesmas Kedungkandang \n");
//  printer.setSize('S');
//  printer.boldOff();
//  printer.println("Jl. Raya Ki Ageng Gribig No.142, Kedungkandang, Kota Malang\n");
  printer.setSize('M');
  printer.println("Poli Gigi \n");
//  printer.boldOn();
  printer.setSize('L');
  printer.println(val_C);
  printer.println("");
  
  
  printer.setSize('S');
  printer.println("Terima kasih sudah mengantri \n\n\n"); 
//  printer.boldOff();
//  antrian = antrian + 1;
  printer.wake();
  printer.setDefault();
  antrian_B = 0; 
  antrian_C +=1;
  EEPROM.write(addr2,antrian_C);
  EEPROM.commit();
//  EEPROM.write(addr,antrian_C); 
  
  }

void loker_D(){

  printer.justify('C');
  
  printer.boldOn();
  printer.setSize('M');
  printer.println("Puskesmas Kedungkandang \n");
//  printer.setSize('S');
//  printer.boldOff();
//  printer.println("Jl. Raya Ki Ageng Gribig No.142, Kedungkandang, Kota Malang\n");
  printer.setSize('M');
  printer.println(" Poli KIA\n");
//  printer.boldOn();
  printer.setSize('L');
  printer.println(val_C);
  printer.println("");
  
  
  printer.setSize('S');
  printer.println("Terima kasih sudah mengantri \n\n\n"); 
//  printer.boldOff();
//  antrian = antrian + 1;
  printer.wake();
  printer.setDefault();
  antrian_B = 0; 
  antrian_C +=1;
  EEPROM.write(addr2,antrian_C);
  EEPROM.commit();
//  EEPROM.write(addr,antrian_C); 
  }

void loker_E(){

  printer.justify('C');
  
  printer.boldOn();
  printer.setSize('M');
  printer.println("Puskesmas Kedungkandang \n");
//  printer.setSize('S');
//  printer.boldOff();
//  printer.println("Jl. Raya Ki Ageng Gribig No.142, Kedungkandang, Kota Malang\n");
  printer.setSize('M');
  printer.println("Poli Infeksius \n");
//  printer.boldOn();
  printer.setSize('L');
  printer.println(val_C);
  printer.println("");
  
  
  printer.setSize('S');
  printer.println("Terima kasih sudah mengantri \n\n\n"); 
//  printer.boldOff();
//  antrian = antrian + 1;
  printer.wake();
  printer.setDefault();
  antrian_B = 0;
  antrian_C +=1;
  EEPROM.write(addr2,antrian_C);
  EEPROM.commit();
//  EEPROM.write(addr,antrian_C); 
  }

void loker_F(){

  printer.justify('C');
  
  printer.boldOn();
  printer.setSize('M');
  printer.println("Puskesmas Kedungkandang \n");
//  printer.setSize('S');
//  printer.boldOff();
//  printer.println("Jl. Raya Ki Ageng Gribig No.142, Kedungkandang, Kota Malang\n");
  printer.setSize('M');
  printer.println("Poli TB \n");
//  printer.boldOn();
  printer.setSize('L');
  printer.println(val_C);
  printer.println("");
  
  
  printer.setSize('S');
  printer.println("Terima kasih sudah mengantri \n\n\n"); 
//  printer.boldOff();
//  antrian = antrian + 1;
  printer.wake();
  printer.setDefault();
  antrian_B = 0; 
  antrian_C +=1;
  EEPROM.write(addr2,antrian_C);
  EEPROM.commit();
//  EEPROM.write(addr,antrian_C); 
  }

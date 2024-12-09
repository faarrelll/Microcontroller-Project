 //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Test P10
/*
 * Original source code : https://github.com/busel7/DMDESP/blob/master/examples/TeksDiamdanJalan/TeksDiamdanJalan.ino by  busel7
 * Links to download libraries : https://github.com/busel7/DMDESP
*/

//----------------------------------------Include Library
//----------------------------------------see here: https://www.youtube.com/watch?v=8jMr94B8iN0 to add NodeMCU ESP8266 library and board
#include <DMDESP.h>
#include <EEPROM.h>
#include <fonts/ElektronMart6x8.h>
#include <fonts/Mono5x7.h>
#include <espnow.h>
#include <ESP8266WiFi.h>
//----------------------------------------

//----------------------------------------DMD Configuration (P10 Panel)
#define DISPLAYS_WIDE 2 //--> Panel Columns
#define DISPLAYS_HIGH 1 //--> Panel Rows
DMDESP Disp(DISPLAYS_WIDE, DISPLAYS_HIGH);  //--> Number of Panels P10 used (Column, Row)
//----------------------------------------

//========================================================================VOID SETUP()

int no_antrian =5 ;
int counter = 5;
String tampilan;
int addr = 0;
typedef struct struct_message{
  
  int ngeprint;
  int antrian;
  
  } struct_message;
  
struct_message myData;
void OnDataRecv(uint8_t * mac, uint8_t *incomingData,  uint8_t len){
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Byte received: ");
  Serial.println(len);
  Serial.print("data1: ");
  Serial.println(myData.ngeprint);
  Serial.print("data2: ");
  Serial.println(myData.antrian);
  no_antrian = myData.antrian;
  if(myData.ngeprint == 10){
    no_antrian = 5;
    }
  EEPROM.write(addr,no_antrian);
  EEPROM.commit();
  
  
  }

void setup() {
  //----------------------------------------DMDESP Setup
  WiFi.mode(WIFI_STA);
  EEPROM.begin(64);
  Serial.begin(115200);
  no_antrian = EEPROM.read(addr);
  if(esp_now_init() != ERR_OK){
    Serial.println("Error initializing ESP-NOW");
    return;
    }

  esp_now_register_recv_cb(OnDataRecv);
  Disp.start(); //--> Run the DMDESP library
  Disp.setBrightness(100); //--> Brightness level
  Disp.setFont(Mono5x7); //--> Determine the font used
  //----------------------------------------
}
//========================================================================

//========================================================================VOID LOOP()
void loop() {
  sortis_antrian();
  if (no_antrian == 5 ){
  Disp.loop(); //--> Run "Disp.loop" to refresh the LED
//  Disp.drawText(0, 9, "Mohon Tunggu Sebentar"); //--> Display text "Disp.drawText(x position, y position, text)"
  Disp.drawText(0, 0, "  ANTRIAN     ");
//  Scrolling_Text(0,80); //--> Show running text "Scrolling_Text(y position, speed);"
  Scrolling_Text2(9,80);
    } else{
  Disp.loop(); //--> Run "Disp.loop" to refresh the LED
  Disp.drawText(0, 9, tampilan); //--> Display text "Disp.drawText(x position, y position, text)"
//  Disp.drawText(0, 9, "001->A");
  Scrolling_Text(0,70); //--> Show running text "Scrolling_Text(y position, speed);"
    }
}
//========================================================================

//========================================================================Subroutines for scrolling Text
static char *Text[] = {"ANTRIAN SKRINING PUSKESMAS KEDUNGKANDANG ","Tekan Tombol Tiket Untuk Mendapatkan Tiket Antrian"};

void Scrolling_Text(int y, uint8_t scrolling_speed) {
  static uint32_t pM;
  static uint32_t x;
  int width = Disp.width();
  Disp.setFont(Mono5x7);
  int fullScroll = Disp.textWidth(Text[0]) + width;
  if((millis() - pM) > scrolling_speed) { 
    pM = millis();
    if (x < fullScroll) {
      ++x;
    } else {
      x = 0;
      return;
    }
    Disp.drawText(width - x, y, Text[0]);
  }  
}
void Scrolling_Text2(int y2, uint8_t scrolling_speed2) {
  static uint32_t pM2;
  static uint32_t x2;
  int width2 = Disp.width();
  Disp.setFont(Mono5x7);
  int fullScroll2 = Disp.textWidth(Text[1]) + width2;
  if((millis() - pM2) > scrolling_speed2) { 
    pM2 = millis();
    if (x2 < fullScroll2) {
      ++x2;
    } else {
      x2 = 0;
      return;
    }
    Disp.drawText(width2 - x2, y2, Text[1]);
  }  
}
void sortis_antrian(){

  if (no_antrian < 100 && no_antrian < 10){
    tampilan = " 00" + String(no_antrian) + ">LOKET   ";
  }
 else if (no_antrian >= 10 && no_antrian < 100) {
    tampilan = " 0" + String(no_antrian)+ ">LOKET   ";
  } else {
    tampilan = " "+ String(no_antrian)+ ">LOKET   ";
    }
  
  }

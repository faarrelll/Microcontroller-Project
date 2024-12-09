#include <esp_now.h>
#include <WiFi.h>
#include <TM1637Display.h>
#include <EEPROM.h>
#define buttonA 5
#define buttonB 19
#define buttonC 23
#define buttonD 17
#define buttonE 22
#define buttonNext 25
#define buttonPrev 26

const int CLK = 18; //Set the CLK pin connection to the display
const int DIO = 4; //Set the DIO pin connection to the display

//uint8_t broadcastAddress[] =  {0x7C, 0x9E, 0xBD, 0xE3, 0xAE, 0x0C}; // sound
uint8_t broadcastAddress[] =  {0xC0, 0x49, 0xEF, 0xD1, 0x5D, 0x78}; // sound
//7C:9E:BD:E3:AE:0C

uint8_t broadcastAddress2[] =  {0xF4, 0xCF, 0xA2, 0x7E, 0x14, 0x4F}; // p10led
//uint8_t broadcastAddress3[] =  {0xF0, 0x08, 0xD1, 0xD2, 0x5D, 0xF8};
//uint8_t broadcastAddress3[] =  {0xC0, 0x49, 0xEF, 0xD0, 0x37, 0x80}; //print_thermal
//uint8_t broadcastAddress3[] =  {0xE8, 0xDB, 0x84, 0x11, 0xC5, 0x58}; //print_thermal
uint8_t broadcastAddress3[] =  {0x40, 0x22, 0xD8, 0x5E, 0x8D, 0xCC}; //print_thermal
//print baru wemos : 40:22:D8:5E:8D:CC
//uint8_t broadcastAddress3[] =  {0x8C, 0xAA, 0xB5, 0x86, 0x45, 0x8C}; //print_thermal
//C0:49:EF:D0:37:80
//8C:AA:B5:86:45:8C
//E8:DB:84:11:C5:58






int counter = 5;
int batasmax;
int addr = 0;
int addr2 = 1;

const uint8_t hbis[] = {
  SEG_F | SEG_G | SEG_E | SEG_C ,           // h
  SEG_F | SEG_E | SEG_D | SEG_C | SEG_G,   // b
  SEG_F | SEG_E,                           // i
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D            // s
  };

TM1637Display display(CLK, DIO); //set up the 4-Digit Display.

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
  memcpy(&batas, incomingData, sizeof(batas));
  Serial.print("Byte received: ");
  Serial.println(len);
  Serial.print("Int: ");
  Serial.println(batas.batasmax);
//  val_B = myData.antrian;
   batasmax = batas.batasmax;
   EEPROM.write(addr2,batasmax);
   EEPROM.commit();
//   batasmax = EEPROM.read(addr);
//   Serial.println(batasmax);
  }
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  EEPROM.begin(64);
  WiFi.mode(WIFI_STA);
  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonB, INPUT_PULLUP);
  pinMode(buttonC, INPUT_PULLUP);
  pinMode(buttonD, INPUT_PULLUP);
  pinMode(buttonE, INPUT_PULLUP);
  pinMode(buttonNext, INPUT_PULLUP);
  pinMode(buttonPrev, INPUT_PULLUP);
  batasmax = EEPROM.read(addr2);
  counter = EEPROM.read(addr);
  Serial.println(batasmax); 
 
  if(esp_now_init() != ESP_OK){
      Serial.println("Error initializing");
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
   memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
   if(esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
    }
   memcpy(peerInfo.peer_addr, broadcastAddress3, 6);
   if(esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
    }

   display.setBrightness(0x0a);
    
}

void loop() {
  // put your main code here, to run repeatedly:
  int btn_A = digitalRead(buttonA);
  int btn_B = digitalRead(buttonB);
  int btn_C = digitalRead(buttonC);
  int btn_D = digitalRead(buttonD);
  int btn_E = digitalRead(buttonE);
  int btn_N = digitalRead(buttonNext);
  int btn_P = digitalRead(buttonPrev);
  if(counter == 5){
      display.setSegments(hbis);
    }else{
        display.showNumberDec(counter);
      }
  
  
  // led dan sound
  
  if(btn_N == 0 & btn_P == 0){

    counter = 5;
    EEPROM.write(addr,counter);
    EEPROM.commit();
    myData.antrian = counter;
    myData.ngeprint = 10 ; 
    esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
    
    }
  else if(counter == batasmax){
      
      if(btn_P == 0){
        myData.antrian = counter;
        Serial.print("panggil ulang :");
        Serial.println(counter); 
        myData.ngeprint =0 ; 
        esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
        Serial.print("Data Suara: ");
        Serial.print(String(myData.antrian));
      
      }
    
    } else if(counter < batasmax){
  if(btn_N == 0){
      if (counter >= 5){
       
        counter += 1;
        myData.antrian = counter;
        Serial.print("cnt + :");
        Serial.println(counter); 
        myData.ngeprint =0 ; 
        esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
        Serial.print("Data Suara: ");
        Serial.print(String(myData.antrian));
        EEPROM.write(addr,counter);
        EEPROM.commit();
        display.showNumberDec(counter);
        delay(4000);
        
      
      }
     
    } else if(btn_P == 0){

        myData.antrian = counter;
        Serial.print("panggil ulang :");
        Serial.println(counter); 
        myData.ngeprint =0 ; 
        esp_err_t result = esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
        Serial.print("Data Suara: ");
        Serial.print(String(myData.antrian));
        EEPROM.write(addr,counter);
        EEPROM.commit();
        
      
      }

    
    }
    

      // prinnter
        if(btn_A == 0){
      myData.ngeprint =1 ; 
      esp_err_t result = esp_now_send(broadcastAddress3, (uint8_t *) &myData, sizeof(myData));

      
    } else if (btn_B == 0){
      myData.ngeprint = 2 ;
      esp_err_t result = esp_now_send(broadcastAddress3, (uint8_t *) &myData, sizeof(myData));

        
      } else if (btn_C == 0){
      myData.ngeprint = 3 ;
      esp_err_t result = esp_now_send(broadcastAddress3, (uint8_t *) &myData, sizeof(myData));

        
      } else if (btn_D == 0){
      myData.ngeprint = 4 ;
      esp_err_t result = esp_now_send(broadcastAddress3, (uint8_t *) &myData, sizeof(myData));

        
      } else if (btn_E == 0){
      myData.ngeprint = 5 ;
      esp_err_t result = esp_now_send(broadcastAddress3, (uint8_t *) &myData, sizeof(myData));


      }

    delay(200);

}

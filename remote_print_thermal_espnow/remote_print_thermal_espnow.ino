#include <esp_now.h>
#include <WiFi.h>
#define buttonA 5
#define buttonB 19
#define buttonC 23
#define buttonD 4
#define buttonE 18
#define buttonNext 25
#define buttonPrev 26

uint8_t broadcastAddress[] =  {0xF0, 0x08, 0xD1, 0xD2, 0x5D, 0xF8};
//uint8_t broadcastAddress2[] = {0x24, 0xA1, 0x60, 0x30, 0x57, 0x30};
//uint8_t broadcastAddress3[] = {0xF0, 0x08, 0xD1, 0xD2, 0x5D, 0xF8};

typedef struct struct_message{
  
//  int antrian_suara;
  int ngeprint;
  int antrian;
  
  } struct_message;
  

struct_message myData;

esp_now_peer_info_t peerInfo;

void OnDataSent (const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  }
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
   pinMode(buttonA, INPUT_PULLUP);
    pinMode(buttonB, INPUT_PULLUP);
    pinMode(buttonC, INPUT_PULLUP);
    pinMode(buttonD, INPUT_PULLUP);
    pinMode(buttonE, INPUT_PULLUP);
    pinMode(buttonNext, INPUT_PULLUP);
    pinMode(buttonPrev, INPUT_PULLUP);
  WiFi.mode(WIFI_STA);
  if(esp_now_init() != ESP_OK){
      Serial.println("Error initializing");
      return;
    }
   esp_now_register_send_cb(OnDataSent);
   peerInfo.channel = 0;
   peerInfo.encrypt = false;
   memcpy(peerInfo.peer_addr, broadcastAddress, 6);
   if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
//  memcpy(peerInfo.peer_addr, broadcastAddress2, 6);
//   if (esp_now_add_peer(&peerInfo) != ESP_OK){
//    Serial.println("Failed to add peer");
//    return;
//  }
//  memcpy(peerInfo.peer_addr, broadcastAddress3, 6);
//   if (esp_now_add_peer(&peerInfo) != ESP_OK){
//    Serial.println("Failed to add peer");
//    return;
//  }
   
   if(esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
    }

   
//    myData.antrian_suara =5;
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
  if(btn_N == 0){
     myData.antrian =1 ; 
     esp_err_t result2 = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

     
     Serial.print("Data Suara: ");
     Serial.print(String(myData.antrian));
    } else if(btn_P == 0){
       myData.antrian =2;
       esp_err_t result2 = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  
     Serial.print("Data Suara 2 : ");
     Serial.print(String(myData.antrian));
      }
//  if(btn_A == 0){
//      myData.ngeprint =1 ; 
//      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
//
//      
//    } else if (btn_B == 0){
//      myData.ngeprint = 2 ;
//      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
//
//        
//      } else if (btn_C == 0){
//      myData.ngeprint = 3 ;
//      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
//
//        
//      } else if (btn_D == 0){
//      myData.ngeprint = 4 ;
//      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
//
//        
//      } else if (btn_E == 0){
//      myData.ngeprint = 5 ;
//      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
//
//
//      }
     
  

//  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
//  if(result == ESP_OK){
//    Serial.println("Sent With Success");
//    }
//   else{
//    Serial.println("Error sending the data");
//    }
    delay(100);

}

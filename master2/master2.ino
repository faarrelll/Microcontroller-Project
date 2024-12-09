#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] =  {0xF0, 0x08, 0xD1, 0xD2, 0x5D, 0xF8};
typedef struct struct_message{
  
  int ketinggian_air1;
  int kelembapan_tanah1;
  
  } struct_message;

typedef struct struct_message2{
  
  int ketinggian_air1;
  int kelembapan_tanah1;
  int ketinggian_air2;
  int kelembapan_tanah2;
  
  } struct_message2;

esp_now_peer_info_t peerInfo;
struct_message2 myData2;
struct_message myData;

int nilai_ketinggian;
int nilai_kelembapan;

void OnDataSent (const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  }
  
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len){
  memcpy(&myData, incomingData, sizeof(myData));
  pembacaan_sensor();
  myData2.ketinggian_air1 = myData.ketinggian_air2;
  myData2.kelembapan_tanah1 = myData.ketinggian_air2;
  myData2.ketinggian_air2 = nilai_ketinggian;
  myData2.kelembapan_tanah2 = nilai_kelembapan;
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData2, sizeof(myData2));
  if(result == ESP_OK){
    Serial.println("Sent With Success");
    }
   else{
    Serial.println("Error sending the data");
    }
 
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
  esp_now_register_send_cb(OnDataSent);
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
   peerInfo.channel = 0;
   peerInfo.encrypt = false;

   if(esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
    }

}

void loop() {
  // put your main code here, to run repeatedly:

}

void pembacaan_sensor(){
  
  }

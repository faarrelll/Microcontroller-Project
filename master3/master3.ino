#include <esp_now.h>
#include <WiFi.h>


typedef struct struct_message2{
  
  int ketinggian_air1;
  int kelembapan_tanah1;
  int ketinggian_air2;
  int kelembapan_tanah2;
  
  } struct_message2;


struct_message2 myData2;


int nilai_ketinggian1;
int nilai_kelembapan1;

int nilai_ketinggian2;
int nilai_kelembapan2;


int nilai_ketinggian3;
int nilai_kelembapan3;


  
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len){
  memcpy(&myData, incomingData, sizeof(myData));
  pembacaan_sensor();

  nilai_ketinggian1 = myData2.ketinggian_air1;
  nilai_kelembapan1 = myData2.kelembapan_tanah1;
  nilai_ketinggian2 = myData2.ketinggian_air2;
  nilai_kelembapan2 = myData2.kelembapan_tanah2;
  nilai_ketinggian3 = ketinggian();
  nilai_kelembapan3 = kelembapan();
  kirim_data();
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  if(esp_now_init() != ESP_OK){
    Serial.println("Error initializing ESP-NOW");
    return;
    }

  esp_now_register_recv_cb(OnDataRecv);

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

int ketinggian(){
  
  }

int kelembapan(){
  
  }
}

#include <espnow.h>
#include <ESP8266WiFi.h>

typedef struct struct_message{
  
  int ngeprint;
  
  } struct_message;
struct_message myData;
void OnDataRecv(uint8_t * mac, uint8_t *incomingData,  uint8_t len){
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Byte received: ");
  Serial.println(len);
  Serial.print("data: ");
  Serial.println(myData.ngeprint);
  
  }
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  
  if(esp_now_init() != ERR_OK){
    Serial.println("Error initializing ESP-NOW");
    return;
    }

  esp_now_register_recv_cb(OnDataRecv);

}

void loop() {
  // put your main code here, to run repeatedly:

}

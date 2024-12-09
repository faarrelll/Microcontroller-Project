#include <Arduino.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h"
#define mybutton 18
#define mybutton2 19

AudioGeneratorMP3 *mp3;
AudioFileSourceSPIFFS *file;
AudioOutputI2SNoDAC *out;
AudioFileSourceID3 *id3;

void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string) {
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
int i = 1;

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

void setup() {
  // WiFi.forceSleepBegin();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
   if(esp_now_init() != ESP_OK){
    Serial.println("Error initializing ESP-NOW");
    return;
    }

  esp_now_register_recv_cb(OnDataRecv);
  SPIFFS.begin();

  pinMode(mybutton, INPUT_PULLUP);
  pinMode(mybutton2, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(mybutton) == 0) {
    while (digitalRead(mybutton) == 0) { delay(10); }
    i++;
  
  if(i%2 == 0 ){
    mulai_musik("/na.mp3");
    delay(500);
    mulai_musik("/1.mp3");
    delay(500);
    mulai_musik("/sumber_suara.mp3");
    delay(500);
  }else{
    mulai_musik("/na.mp3");
    delay(500);
    mulai_musik("/2.mp3");
    delay(500);
    mulai_musik("/sumber_suara.mp3");
    delay(500);
  }
  }
}

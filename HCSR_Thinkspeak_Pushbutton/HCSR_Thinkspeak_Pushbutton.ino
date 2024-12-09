#include <WiFi.h>
#include <ThingSpeak.h>

#define triggerPin  5
#define echoPin     18
#define push_button 4
#define led 13
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

const char* ssid = "Hai";
const char* password = "hai12345";

WiFiClient client;

unsigned long myChannelNumber = 1;
const char* myWriteAPIKey = "E9B51JJU8L127EFA";



long duration;
float distanceCm;
float distanceInch;

void setup() {
  Serial.begin(115200);
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(led, OUTPUT);
  pinMode(push_button, INPUT_PULLUP);
  WiFi.mode(WIFI_STA);
  connectWiFi();
  ThingSpeak.begin(client);

}

void loop() {
  // put your main code here, to run repeatedly:
      if(WiFi.status() == WL_CONNECTED){
  
      
      int value_button = digitalRead(push_button);
      if(value_button == LOW){
        
        sendData();
        digitalWrite(led,HIGH);
        delay(20000);
        digitalWrite(led,LOW);
      }
      
    }else {
      connectWiFi();
    }
    
    

    
  
}
void connectWiFi(){
   if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid,password);
        delay(5000);
      }
      Serial.println("\nConnected.");
    }
}

void sensorRead(){
  digitalWrite(triggerPin, LOW);
    delayMicroseconds(2); 
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10); 
    digitalWrite(triggerPin, LOW);

    duration = pulseIn(echoPin, HIGH);

    distanceCm = duration*SOUND_SPEED/2;

    distanceInch = distanceCm * CM_TO_INCH;
}

void sendData(){
    sensorRead();
    ThingSpeak.setField(1, distanceCm);
    ThingSpeak.setField(2, distanceInch);
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if(x == 200){
    Serial.println("Channel update successful.");
    } else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

}


#include <WiFi.h>
#include <ArduinoJson.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>
#include <HardwareSerial.h>
HardwareSerial mySerial(Serial2); // RX, TX
JsonDocument doc;

int pump = 26;
int selenoid1 = 19;
int selenoid2 = 23;
int selenoid3 = 27;


#define DATABASE_URL "sawahpintar-2923f-default-rtdb.asia-southeast1.firebasedatabase.app"  //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "36pdBs2I6H1skjaOgGWlnthmn3BrYOZDhsZZMxan"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
// FirebaseJson json;

const char* ssid = "OCEAN";
const char* password = "cerdas2023";
bool pompa,umur;
int ka1,ka2,ka3,kt1,kt2,kt3;
int t1,t2,t3,l1,l2,l3;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  mySerial.begin(115200, SERIAL_8N1, 16, 17);
  pinMode(pump,OUTPUT);
  pinMode(selenoid1,OUTPUT);
  pinMode(selenoid2,OUTPUT);
  pinMode(selenoid3,OUTPUT);
  WiFi.begin(ssid, password);


while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
}
 Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

    config.database_url = DATABASE_URL;
    config.signer.tokens.legacy_token = DATABASE_SECRET;
    Firebase.reconnectWiFi(true);
    Firebase.begin(&config, &auth);

}

void loop() {
  // put your main code here, to run repeatedly:
   if (mySerial.available() > 0) {
    FirebaseJson json;
    String cek= mySerial.readString();
   deserializeJson(doc, cek);

   kt1 = doc["kt1"]; // 1
   kt2 = doc["kt2"]; // 2
   kt3 = doc["kt3"]; // 3
   ka1 = doc["ka1"]; // 1
   ka2 = doc["ka2"]; // 2
   ka3 = doc["ka3"]; // 3

   t1 = map(ka1, 0,17,17,0);
   t2 = map(ka2, 0,17,17,0);
   t3 = map(ka3, 0,17,17,0);

   
   Serial.print("kt 1: ");
   Serial.println(kt1);
   Serial.print("kt 2: ");
   Serial.println(kt2);
   Serial.print("kt 3: ");
   Serial.println(kt3);
   Serial.print("ka 1: ");
   Serial.println(ka1);
   Serial.print("ka 2: ");
   Serial.println(ka2);
   Serial.print("ka 3: ");
   Serial.println(ka3);

  json.add("Paddy/kt1", kt1);
  json.add("Paddy/kt2", kt2);
  json.add("Paddy/kt3", kt3);
  json.add("Paddy/ka1", ka1);
  json.add("Paddy/ka2", ka2);
  json.add("Paddy/ka3", ka3);

  Serial.printf("Update node... %s\n", Firebase.RTDB.updateNode(&fbdo, F("/"), &json) ? "ok" : fbdo.errorReason().c_str());
  }
   Firebase.RTDB.getJSON(&fbdo,"Paddy");
   String cek = fbdo.to<String>();
   StaticJsonDocument<1024> doc2;
    deserializeJson(doc2, cek);
   JsonObject obj = doc2.as<JsonObject>();
   pompa = obj["pompa"];
   umur = obj["age"];
   // Serial.print(cek);
//   Serial.print("Pompa : ");
//   Serial.println(pompa);
//   Serial.print("umur : ");
//   Serial.println(umur);

  if(pompa == 1){
  if(umur == 0 ){

    if(t1 <= 0 ){
      digitalWrite(pump,HIGH);
      digitalWrite(selenoid1,HIGH);
    } else if( t1 >= 4){
      digitalWrite(selenoid1,LOW);
    }

    if(t2 <= 0 ){
      digitalWrite(pump,HIGH);
      digitalWrite(selenoid2,HIGH);
    } else if( t2 >= 4){
      digitalWrite(selenoid1,LOW);
    }

    if(t2 <= 0 ){
      digitalWrite(pump,HIGH);
      digitalWrite(selenoid2,HIGH);
    } else if( t2 >= 4){
      digitalWrite(selenoid1,LOW);
    }

    if(t1 >= 4 && t2 >= 4 && t3 >= 4){
      digitalWrite(pump,LOW);
      }
    
    }
  else{
    
    if(l1 <= 40 ){
      digitalWrite(pump,HIGH);
      digitalWrite(selenoid1,HIGH);
    } else if( l1 >= 60){
      digitalWrite(selenoid1,LOW);
    }

    if(l2 <= 40 ){
      digitalWrite(pump,HIGH);
      digitalWrite(selenoid2,HIGH);
    } else if( l2 >= 60){
      digitalWrite(selenoid1,LOW);
    }

    if(l2 <= 40 ){
      digitalWrite(pump,HIGH);
      digitalWrite(selenoid2,HIGH);
    } else if( l2 >= 60){
      digitalWrite(selenoid1,LOW);
    }

    if(l1 >= 60 && l2 >= 60 && l3 >= 60){
      digitalWrite(pump,LOW);
      }
  }
  } else {
    digitalWrite(pump,LOW);
    
    }
    
    
    
    


  
  
  
  
  
  
  
  
  
  
  
  



}

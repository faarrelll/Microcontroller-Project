#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
#include "DFRobotDFPlayerMini.h"
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <ArduinoJson.h>
#include <addons/RTDBHelper.h>
#include <TinyGPSPlus.h>

#define DATABASE_URL "mobil-listrik-98dbd-default-rtdb.asia-southeast1.firebasedatabase.app"  //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define DATABASE_SECRET "i1ZZaxiqDAXR7VGep42GLYbxJJFom1520gLaqJFK"

#define pwma 2   // 25 corresponds to GPIO16
#define pwmb 4   // 26 corresponds to GPIO17
#define pwmc 12  // 17 corresponds to GPIO5
#define pwmd 32
#define sein_kiri 19
#define sein_kanan 22
//#define klakson 26
#define lampu_utama 23
#define lampu_belakang 25

AsyncWebServer server(80);
HardwareSerial mySoftwareSerial(Serial1);  // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
int isFinished = 1;

// Create AsyncWebServer object on port 80

TinyGPSPlus gps;
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Search for parameter in HTTP POST request
const char* PARAM_INPUT_1 = "ssid";
const char* PARAM_INPUT_2 = "pass";
const char* PARAM_INPUT_3 = "ip";
const char* PARAM_INPUT_4 = "gateway";

//Variables to save values from HTML form
String ssid;
String pass;
String ip;
String gateway;

// File paths to save input values permanently
const char* ssidPath = "/ssid.txt";
const char* passPath = "/pass.txt";
const char* ipPath = "/ip.txt";
const char* gatewayPath = "/gateway.txt";

bool satu, dua, tiga, empat, lima;
int kecepatan, belok, maju, mundur, kanan, kiri;


volatile bool dataChanged = false;
int treshold = 130;


int pwma_DutyCycle;
int pwmb_DutyCycle;
int pwmc_DutyCycle;
int pwmd_DutyCycle;

const int pwma_PWMFreq = 30000;
const int pwma_PWMChannel = 0;
const int pwma_PWMResolution = 8;
//const int RED_MAX_DUTY_CYCLE = (int)(pow(2, redPWMResolution) - 1);

const int pwmb_PWMFreq = 30 ;
const int pwmb_PWMChannel = 1;
const int pwmb_PWMResolution = 8;
//const int GREEN_MAX_DUTY_CYCLE = (int)(pow(2, greenPWMResolution) - 1);

const int pwmc_PWMFreq = 30000;
const int pwmc_PWMChannel = 2;
const int pwmc_PWMResolution = 8; 
//const int BLUE_MAX_DUTY_CYCLE = (int)(pow(2, bluePWMResolution) - 1);

const int pwmd_PWMFreq = 30000;
const int pwmd_PWMChannel = 3;
const int pwmd_PWMResolution = 8;
//const int BLUE_MAX_DUTY_CYCLE = (int)(pow(2, bluePWMResolution) - 1);

IPAddress localIP;
//IPAddress localIP(192, 168, 1, 200); // hardcoded

// Set your Gateway IP address
IPAddress localGateway;
//IPAddress localGateway(192, 168, 1, 1); //hardcoded
IPAddress subnet(255, 255, 0, 0);

// Timer variables
unsigned long previousMillis = 0;
const long interval = 10000;  // interval to wait for Wi-Fi connection (milliseconds)

//,sein_kiri,sein_kanan,klakson,lampu_utama

// Initialize SPIFFS
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

// Read File from SPIFFS
String readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\r\n", path);

  File file = fs.open(path);
  if(!file || file.isDirectory()){
    Serial.println("- failed to open file for reading");
    return String();
  }
  
  String fileContent;
  while(file.available()){
    fileContent = file.readStringUntil('\n');
    break;     
  }
  return fileContent;
}
// Write file to SPIFFS
void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\r\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("- failed to open file for writing");
    return;
  }
  if(file.print(message)){
    Serial.println("- file written");
  } else {
    Serial.println("- write failed");
  }
}


// Initialize WiFi
bool initWiFi() {
  if(ssid=="" || ip==""){
    Serial.println("Undefined SSID or IP address.");
    return false;
  }

  WiFi.mode(WIFI_STA);
  // localIP.fromString(ip.c_str());
  // localGateway.fromString(gateway.c_str());


  if (!WiFi.config(localIP, localGateway, subnet)){
    Serial.println("STA Failed to configure");
    return false;
  }
  WiFi.begin(ssid.c_str(), pass.c_str());
  Serial.println("Connecting to WiFi...");

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;

  while(WiFi.status() != WL_CONNECTED) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      Serial.println("Failed to connect.");
      return false;
    }
  }

  Serial.println(WiFi.localIP());
  return true;
}


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  initSPIFFS();
  mySoftwareSerial.begin(9600);
  Serial2.begin(9600);
  
  Serial2.
  // Load values saved in SPIFFS
  ssid = readFile(SPIFFS, ssidPath);
  pass = readFile(SPIFFS, passPath);
  ip = readFile(SPIFFS, ipPath);
  gateway = readFile (SPIFFS, gatewayPath);
  Serial.println(ssid);
  Serial.println(pass);
  Serial.println(ip);
  Serial.println(gateway);

  pinMode(pwma, OUTPUT);
  pinMode(pwmb, OUTPUT);
  pinMode(pwmc, OUTPUT);
  pinMode(pwmd, OUTPUT);
  pinMode(sein_kiri, OUTPUT);
  pinMode(sein_kanan, OUTPUT);
//  pinMode(klakson, OUTPUT);
  pinMode(lampu_utama, OUTPUT);
  pinMode(lampu_belakang, OUTPUT);
  
  
  /* Initialize PWM Channels with Frequency and Resolution */
  ledcSetup(pwma_PWMChannel, pwma_PWMFreq, pwma_PWMResolution);
  ledcSetup(pwmb_PWMChannel, pwmb_PWMFreq, pwmb_PWMResolution);
  ledcSetup(pwmc_PWMChannel, pwmc_PWMFreq, pwmc_PWMResolution);
  ledcSetup(pwmd_PWMChannel, pwmd_PWMFreq, pwmd_PWMResolution);
  /* Attach the LED PWM Channel to the GPIO Pin */
  ledcAttachPin(pwma, pwma_PWMChannel);
  ledcAttachPin(pwmb, pwmb_PWMChannel);
  ledcAttachPin(pwmc, pwmc_PWMChannel);
  ledcAttachPin(pwmd, pwmd_PWMChannel);
  

 

  if(initWiFi()) {
    Serial.println("Connect cok");
    Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
    config.database_url = DATABASE_URL;
    config.signer.tokens.legacy_token = DATABASE_SECRET;
  
    Firebase.reconnectWiFi(true);
    Firebase.begin(&config, &auth);

    Serial.println(F("DFRobot DFPlayer Mini Demo"));
    Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

//   if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
//    Serial.println(F("Unable to begin:"));
//    Serial.println(F("1.Please recheck the connection!"));
//    Serial.println(F("2.Please insert the SD card!"));
//    while (true) {
//      
//      delay(0);  // Code to compatible with ESP8266 watch dog.
//      }
//    }
    while(!myDFPlayer.begin(mySoftwareSerial)){
      Serial.println(F("Unable to begin:"));
      Serial.println(F("1.Please recheck the connection!"));
      Serial.println(F("2.Please insert the SD card!"));
      digitalWrite(lampu_belakang, HIGH);
      delay(500);
      digitalWrite(lampu_belakang, LOW);
      delay(500);
      digitalWrite(lampu_belakang, HIGH);
      delay(1000);
      digitalWrite(lampu_belakang, LOW);
      delay(1000);
      
      };
    Serial.println(F("DFPlayer Mini online."));
//
    myDFPlayer.volume(30); 
  }
  else {
    // Connect to Wi-Fi network with SSID and password
    Serial.println("Setting AP (Access Point)");
    // NULL sets an open Access Point
    WiFi.softAP("Mobil Mainan", NULL);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP); 

    // Web Server Root URL
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send(SPIFFS, "/wifimanager.html", "text/html");
    });
    
    server.serveStatic("/", SPIFFS, "/");
    
    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {
      int params = request->params();
      for(int i=0;i<params;i++){
        AsyncWebParameter* p = request->getParam(i);
        if(p->isPost()){
          // HTTP POST ssid value
          if (p->name() == PARAM_INPUT_1) {
            ssid = p->value().c_str();
            Serial.print("SSID set to: ");
            Serial.println(ssid);
            // Write file to save value
            writeFile(SPIFFS, ssidPath, ssid.c_str());
          }
          // HTTP POST pass value
          if (p->name() == PARAM_INPUT_2) {
            pass = p->value().c_str();
            Serial.print("Password set to: ");
            Serial.println(pass);
            // Write file to save value
            writeFile(SPIFFS, passPath, pass.c_str());
          }
          // HTTP POST ip value
          if (p->name() == PARAM_INPUT_3) {
            ip = p->value().c_str();
            Serial.print("IP Address set to: ");
            Serial.println(ip);
            // Write file to save value
            writeFile(SPIFFS, ipPath, ip.c_str());
          }
          // HTTP POST gateway value
          if (p->name() == PARAM_INPUT_4) {
            gateway = p->value().c_str();
            Serial.print("Gateway set to: ");
            Serial.println(gateway);
            // Write file to save value
            writeFile(SPIFFS, gatewayPath, gateway.c_str());
          }
          //Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
        }
      }
      request->send(200, "text/plain", "Done. ESP will restart");
      delay(3000);
      ESP.restart();
    });
    server.begin();
  }
}

void loop() {

  if(WiFi.status() == WL_CONNECTED){
    //initWiFi();
    firebase();
    lokasi();
  }

  

  
 

    
}

void firebase(){

  Firebase.RTDB.getJSON(&fbdo, "mobil");
  String cek = fbdo.to<String>();
  //Serial.println(cek);
  StaticJsonDocument<1024> doc;
  deserializeJson(doc, cek);
  JsonObject obj = doc.as<JsonObject>();
  kecepatan = obj["kecepatan"];
  belok = obj["belok"];
  dua = obj["seinkanan"];
  satu = obj["seinkiri"];
  tiga = obj["klakson"];
  empat = obj["lampuutama"];
  lima = obj["lampubelakang"];
  Serial.println(cek);

  if (kecepatan >= 0) {
    mundur = 0;
    maju = kecepatan;
//    digitalWrite(lampu_belakang, HIGH);
//    digitalWrite(lampu_belakang, LOW);
  } else {
    mundur = kecepatan * -1;
    maju = 0;
//    digitalWrite(lampu_belakang, LOW);
  }
  if(kecepatan > 0){
     digitalWrite(lampu_belakang, HIGH);
    } else {
      digitalWrite(lampu_belakang, LOW);
      }
  if (belok >= 0) {
    kiri = 0;
    if (belok >= treshold) {
      belok = treshold;
    }
    kanan = belok;
  } else {
    kanan = 0;
    kiri = belok * -1;
    if (kiri >= treshold) {
      kiri = treshold;
    }
    Serial.println(kiri);
    
  }
  if (tiga == true){
    bacacok(1);
  }
  if(satu == true){
    digitalWrite(sein_kiri, HIGH);
    delay(200);
    digitalWrite(sein_kiri, LOW);
    delay(200);
    } else if(dua == true){
      digitalWrite(sein_kanan, HIGH);
      delay(200);
      digitalWrite(sein_kanan, LOW);
      delay(200);
      } else {
      digitalWrite(sein_kiri, LOW);
      digitalWrite(sein_kanan, LOW);
      }

//   if (satu == true){
//    digitalWrite(sein_kiri, HIGH);
//    } else {
//      digitalWrite(sein_kiri, LOW);
//      }
//
//    if (dua == true){
//    digitalWrite(sein_kanan, HIGH);
//    } else {
//      digitalWrite(sein_kanan, LOW);
//      }
      
  if (empat == true){
    digitalWrite(lampu_utama, HIGH);
    } else {
      digitalWrite(lampu_utama, LOW);
      }
  
  
//  digitalWrite(lampu_belakang, HIGH);
  
  ledcWrite(pwma_PWMChannel, maju);
  ledcWrite(pwmb_PWMChannel, mundur);
  ledcWrite(pwmc_PWMChannel, kanan);
  ledcWrite(pwmd_PWMChannel, kiri);


}

void bacacok(int nomor) {
  Serial.print(isFinished);
  if(isFinished == 1){
      myDFPlayer.play(nomor);
      isFinished =0;
  }
  while (isFinished == 0){
      if (myDFPlayer.available()) {
        printDetail(myDFPlayer.readType(), myDFPlayer.read());
        if(myDFPlayer.readType() == DFPlayerPlayFinished){
          isFinished = 1;
        }
      }
  }
}

void printDetail(uint8_t type, int value) {
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

void lokasi(){
  
   while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
      displayInfo();


      while(millis() > 5000 && gps.charsProcessed() < 10){
        
          Serial.println(F("No GPS detected: check wiring."));
          digitalWrite(lampu_belakang, HIGH);
          delay(500);
          digitalWrite(lampu_belakang, LOW);
          delay(500);
          digitalWrite(lampu_belakang, HIGH);
          delay(500);
          digitalWrite(lampu_belakang, LOW);
          delay(500);
          digitalWrite(lampu_belakang, HIGH);
          delay(1000);
          digitalWrite(lampu_belakang, LOW);
          delay(1000);
        
        };
//  if (millis() > 5000 && gps.charsProcessed() < 10)
//  {
//    Serial.println(F("No GPS detected: check wiring."));
//    do{
//      //lampu kedip2
//      }while (true);
//  }
}

void displayInfo()
{


  Serial.print(F("Location: "));


  if (gps.location.isValid()){


    Serial.print("Lat: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print("Lng: ");
    Serial.print(gps.location.lng(), 6);
    double lat = gps.location.lat();
    double lng = gps.location.lng();
    String LongitudeString = String(lng, 6);
    String LatitudeString = String(lat, 6);
    Firebase.RTDB.setString(&fbdo, "/mobil/lng", LongitudeString);
    Firebase.RTDB.setString(&fbdo, "/mobil/lat", LatitudeString);

    Serial.println();
    firebase();


  }  





}

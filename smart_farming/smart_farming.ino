/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"
#include "DHT.h"
#include <WiFi.h>
#include <PubSubClient.h>
// #include <Wire.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include "Update.h"
#include <WiFiClientSecure.h>
#include <EEPROM.h>
#include "time.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <WiFiProvisioner.h>

#define dhtPin 23
#define echoPin 19
#define trigPin 18
#define mistMakerPin 4
#define irigasiPin1 16
#define irigasiPin2 17
#define soilMoisture 32
#define b1 27
#define b2 14
#define b3 13
#define in1 26
#define in2 25
#define in3 33

#define DHTTYPE DHT21

// Define server details and file path
#define HOST "raw.githubusercontent.com"
#define PATH "/dityokreshna/ota-onlineupdate/main/otafiles.bin"
#define PORT 443

// Define the name for the downloaded firmware file
#define FILE_NAME "firmware.bin"

const char* ssid = "OCEAN";
const char* password = "cerdas2023";

const char* mqtt_server = "mqtt-dev.exiglosi.com";
const char* mqtt_user = "myuser";
const char* mqtt_pass = "m@YpaSsw0rd";
long lastMsg = 0;
char msg[50];

bool pilihan = 0;
int value = 0;
int lcdColumns = 20;
int lcdRows = 4;
long duration;
int distance;
int buttonDelay = 150;
int loading = 0;

unsigned long currentMillis = millis();
const unsigned long erasePeriod = 1000;
static unsigned long erasePrevious = 0;
static unsigned long loadingPrevious = 0;
static unsigned long dhtPrev = 0;
static unsigned long soilPrev = 0;
static unsigned long wtrPrev = 0;
const long utcOffsetInSeconds = 7 * 3600;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
DHT dht(dhtPin, DHTTYPE);
WiFiClient espClient;
PubSubClient client(espClient);
RTC_DS3231 ds3231;
JsonDocument doc;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
WiFiProvisioner::WiFiProvisioner provisioner;

int wtrlvl, soil, capacity;
float temp, hum;
// bool irigasiOut1,irigasiOut2,sprayOut;
String uniqId, tipeData;
bool irigasi1, irigasi2, spray;
bool modeIM, modeIS, modeIA, modeIT, modeMM, modeMS, modeMA, modeMT;
bool btIMLock, btMMLock;
int tempFrom, tempTo, humFrom, humTo, wtrTo;
int tempFromaddr = 0;
int tempToaddr = 1;
int humFromaddr = 2;
int humToaddr = 3;
int wtrToaddr = 4;
String sensorTopic, irigasiTopic, sprayTopic, intemperatureTopic, inhumidityTopic, inwaterTopic, updateTopic;
byte upSymbol[] = { B00000, B00000, B00100, B01010, B10001, B00000, B00000, B00000 };
byte dnSymbol[] = { B00000, B00000, B00000, B10001, B01010, B00100, B00000, B00000 };
byte wifiHigh[] = { B01110, B11111, B10001, B00100, B11111, B10001, B00100, B00100 };
byte wifiMedium[] = { B00000, B00000, B00000, B00100, B11111, B10001, B00100, B00100 };
byte wifiLow[] = { B00000, B00000, B00000, B00000, B00000, B00000, B00100, B00100 };
byte okSymbol[] = { B00000, B00000, B01100, B10010, B10010, B01100, B00000, B00000 };
byte humWarning[] = { B00001, B00001, B01000, B00101, B01100, B11110, B11110, B01100 };
byte tempWarning[] = { B01101, B01101, B01100, B01101, B01100, B10010, B10010, B01100 };

bool checkInputCode(const String& input) {
  if (input == "otomasi") {
    return true;
  } else {
    return false;
  }
}

void setup() {
  Serial.begin(115200);
  initAll();
  pinModeAll();
  EEPROM.begin(64);
  tempFrom = EEPROM.read(tempFromaddr);
  tempTo = EEPROM.read(tempToaddr);
  humFrom = EEPROM.read(humFromaddr);
  humTo = EEPROM.read(humToaddr);
  wtrTo = EEPROM.read(wtrToaddr);
  uniqId = WiFi.macAddress();
  String rootTopic = "esp32/";
  sensorTopic = rootTopic + "sensor/" + uniqId;
  irigasiTopic = rootTopic + "irigasi/" + uniqId;
  sprayTopic = rootTopic + "spray/" + uniqId;
  intemperatureTopic = rootTopic + "indicatorTemperature/" + uniqId;
  inhumidityTopic = rootTopic + "indicatorHumidity/" + uniqId;
  inwaterTopic = rootTopic + "indicatorWater/" + uniqId;
  updateTopic = rootTopic + "update/" + uniqId;
}

void loop() {
  // ds3231.adjust(DateTime())
  // mqttSend("irigasi");
  lcdMenu();
  // lcd.setCursor(1, 0);
  // lcd.print("[Update  Firmware]");
  // lcd.setCursor(3, 1);
  // lcd.print("Please Wait");
  // lcd.setCursor(5, 2);
  // lcd.print("Firmware ? ");
  // lcd.setCursor(6, 3);
  // lcd.print("YES");
  // lcd.setCursor(12, 3);
  // lcd.print("NO");


  // rtc.adjust()




  // if (millis() - erasePrevious >= erasePeriod) {
  //   lcd.clear();
  //   erasePrevious = millis();
  // }
  // Serial.println(erasePrevious)

  // if (digitalRead(b1) == 0) {
  //   digitalWrite(irigasiPin1, HIGH);
  // } else {
  //   digitalWrite(irigasiPin1, LOW);
  // }
  // if (digitalRead(b2) == 0) {
  //   digitalWrite(irigasiPin2, HIGH);
  // } else {
  //   digitalWrite(irigasiPin2, LOW);
  // }
  // if (digitalRead(b3) == 0) {
  //   digitalWrite(mistMakerPin, HIGH);

  // } else {
  //   digitalWrite(mistMakerPin, LOW);
  // }

  // lcd.setCursor(0, 3);
  // lcd.print(uniqId);
  // delay(1000);
  // lcd.setCursor(6, 0);
  // lcd.clear();
}

void mqttSend(String typeKirim) {
  JsonObject doc_object_0;
  JsonObject doc_0 = doc.add<JsonObject>();
  doc_0["ID_UNIC"] = uniqId;
  doc_0["type"] = typeKirim;

  String topicnya;
  String roottopicnya = "server/data";

  if (typeKirim == "sensor") {
    topicnya = roottopicnya;
    doc_object_0 = doc_0["values"].add<JsonObject>();
    doc_object_0["capacity"] = wtrlvl;
    doc_object_0["soil"] = soil;
    doc_object_0["temperature"] = temp;
    doc_object_0["humidity"] = hum;
    doc_object_0["irigasi"] = irigasi1;
    doc_object_0["spray"] = spray;
    doc_object_0["temp_from"] = tempFrom;
    doc_object_0["temp_to"] = tempTo;
    doc_object_0["hum_from"] = humFrom;
    doc_object_0["hum_to"] = humTo;
    doc_object_0["height"] = wtrTo;
  } else if (typeKirim == "irigasi") {
    topicnya = roottopicnya;
    doc_0["values"][0]["pompa"] = irigasi1;
  } else if (typeKirim == "spray") {
    topicnya = roottopicnya;
    doc_0["values"][0]["pompa"] = spray;
  }
  String output;


  doc.shrinkToFit();  // optional
  serializeJson(doc, output);
  int panjang = output.length();

  char kirimData[400];
  output.toCharArray(kirimData, 400);
  Serial.print("Data : ");
  Serial.println(kirimData);

  client.beginPublish(topicnya.c_str(), panjang, false);
  client.print(kirimData);
  client.endPublish();
  doc.clear();
}

void mqttCallback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  JsonDocument doc2;
  Serial.println();
  if (String(topic) == sensorTopic) {
    mqttSend("sensor");
    Serial.println(messageTemp);

  } else if (String(topic) == irigasiTopic) {
    // Serial.println(messageTemp);
    if (messageTemp == "1") {
      irigasi1 = true;
      modeIA = true;
    } else if (messageTemp == "0") {
      irigasi1 = false;
      modeIA = false;
    }
    mqttSend("irigasi");
    Serial.println(irigasi1);
  } else if (String(topic) == sprayTopic) {
    // Serial.println(messageTemp);
    if (messageTemp == "1") {
      spray = true;
      modeMA = true;
    } else if (messageTemp == "0") {
      spray = false;
      modeMA = false;
    }
    mqttSend("spray");
    Serial.println(spray);
  } else if (String(topic) == intemperatureTopic) {
    Serial.println(messageTemp);
    deserializeJson(doc2, messageTemp);
    JsonObject object = doc2.as<JsonObject>();
    tempFrom = object["from"];
    tempTo = object["to"];
    EEPROM.write(tempFromaddr, tempFrom);
    EEPROM.write(tempToaddr, tempTo);
    EEPROM.commit();
    mqttSend("sensor");

  } else if (String(topic) == inhumidityTopic) {
    Serial.println(messageTemp);
    deserializeJson(doc2, messageTemp);
    JsonObject object = doc2.as<JsonObject>();
    humFrom = object["from"];
    humTo = object["to"];
    EEPROM.write(humFromaddr, humFrom);
    EEPROM.write(humToaddr, humTo);
    EEPROM.commit();
    mqttSend("sensor");

  } else if (String(topic) == inwaterTopic) {
    Serial.println(messageTemp);
    deserializeJson(doc2, messageTemp);
    JsonObject object = doc2.as<JsonObject>();
    wtrTo = object["value"];
    EEPROM.write(wtrToaddr, wtrTo);
    EEPROM.commit();
    mqttSend("sensor");
  } else if (String(topic) == updateTopic) {
    Serial.println(messageTemp);
    lcd.clear();
    while (1) {
      lcd.setCursor(1, 0);
      lcd.print("[Update  Firmware]");
      lcd.setCursor(4, 1);
      lcd.print("Please  Wait");
      lcd.setCursor(loading, 2);
      lcd.print(".");
      if (millis() - loadingPrevious >= 500) {
        loading++;
        loadingPrevious = millis();
      }
      if (loading >= 20) {
        lcd.clear();
        loading = 0;
      };
      if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
      }
      getFileFromServer();
      performOTAUpdateFromSPIFFS();
    }
  }

  // if (String(topic) == "esp32/output") {
  //   Serial.print("Changing output to ");
  //   if (messageTemp == "on") {
  //     Serial.println("on");
  //     digitalWrite(ledPin, HIGH);
  //   } else if (messageTemp == "off") {
  //     Serial.println("off");
  //     digitalWrite(ledPin, LOW);
  //   }
  // }
  messageTemp.clear();
}
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("E4:65:B8:D9:FB:48", mqtt_user, mqtt_pass)) {
      // if (client.connect("esp32test")) {
      Serial.println("connected to MQTT Broker");
      client.subscribe(sprayTopic.c_str());
      client.subscribe(irigasiTopic.c_str());
      client.subscribe(sensorTopic.c_str());
      client.subscribe(inwaterTopic.c_str());
      client.subscribe(intemperatureTopic.c_str());
      client.subscribe(inhumidityTopic.c_str());
      client.subscribe(updateTopic.c_str());
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    // client.disconnect();
    }
  }
}

void readWaterLevel() {
   if (millis() - wtrPrev >= 100) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0344 / 2;
  int wtrlvlRead = distance;
  int wtry = map(wtrlvlRead, 22, wtrTo, 100, 0);
  if (wtrlvl < wtry) {
    wtrlvl++;
  } else if (wtrlvl > wtry) {
    wtrlvl--;
  } else {
    wtrlvl = wtrlvl;
  }
  if (wtrlvl > 100) {
    wtrlvl = 100;
  } else if (wtrlvl < 0) {
    wtrlvl = 0;
  }
  wtrPrev = millis();
  }
}

void readDht() {
  if (millis() - dhtPrev >= 100) {
    float tempRead = dht.readTemperature();
    if (temp < tempRead) {
      temp = temp + 0.10;
    } else if (temp > tempRead) {
      temp = temp - 0.10;
    } else {
      temp = tempRead;
    }
    float humRead = dht.readHumidity();
    if (hum < humRead) {
      hum = hum + 0.10;
    } else if (hum > humRead) {
      hum = hum - 0.10;
    } else {
      hum = humRead;
    }
    dhtPrev = millis();
  }
}

void readSoilMoisture() {
   if (millis() - soilPrev >= 100) {
  int soilRead = analogRead(soilMoisture);
  int y = map(soilRead, 0, 2048, 100, 0);
  soil = y;
  if (soil < y) {
    soil++;
  } else if (soil > y) {
    soil--;
  } else {
    soil = soil;
  }

  if (soil > 100) {
    soil = 100;
  } else if (soil < 0) {
    soil = 0;
  }
  soilPrev = millis();
}
}

void initAll() {
  lcd.begin();
  lcd.createChar(0, wifiLow);
  lcd.createChar(1, wifiMedium);
  lcd.createChar(2, wifiHigh);
  lcd.createChar(3, upSymbol);
  lcd.createChar(4, dnSymbol);
  lcd.createChar(5, okSymbol);
  lcd.createChar(6, tempWarning);
  lcd.createChar(7, humWarning);

  dht.begin();
  // if (!ds3231.begin()) {
  //   Serial.println("Could not find ds3231! Check circuit.");
  //   while (1)
  //     ;
  // }
  ds3231.begin();
  provisioner.setInputCheckCallback(checkInputCode);
  provisioner.setShowInputField(true);
  provisioner.AP_NAME = "Tech Garden AP";
  provisioner.SVG_LOGO =
    R"rawliteral(<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 32 32" width="5rem" height="5rem" preserveAspectRatio="xMidYMid meet"><g transform="translate(0 -1020.362)"><circle cx="16" cy="1036.362" r="16" fill="var(--theme-color)" fill-rule="evenodd"></circle><path fill="rgba(0,0,0,0.3)" style="line-height:normal;text-indent:0;text-align:start;text-decoration-line:none;text-decoration-style:solid;text-decoration-color:#000;text-transform:none;block-progression:tb;isolation:auto;mix-blend-mode:normal" d="M24.17 29.734a16 16 0 0 0 .475-.271 16 16 0 0 0 1.3-.93 16 16 0 0 0 1.201-1.054 16 16 0 0 0 1.092-1.172 16 16 0 0 0 .967-1.272 16 16 0 0 0 .836-1.365 16 16 0 0 0 .695-1.44 16 16 0 0 0 .383-1.05l-9.629-9.63-.02-.019A15.39 15.39 0 0 0 10.54 7a.534.534 0 0 0-.54.543.534.534 0 0 0 .541.525l3.467 3.467-.053-.02A11.426 11.426 0 0 0 10.553 11a.546.546 0 0 0-.553.555.546.546 0 0 0 .553.537l4.19 4.19c-.129-.088-.264-.164-.397-.243-.078-.046-.152-.097-.23-.14h-.003a7.392 7.392 0 0 0-.646-.313h-.002a7.37 7.37 0 0 0-.68-.25h-.002a7.328 7.328 0 0 0-2.203-.334.572.572 0 0 0-.58.58.572.572 0 0 0 .58.563l6.275 6.275a.572.572 0 0 0 .573.572l6.742 6.742zm-4.451 1.809a16 16 0 0 0 .56-.127 16 16 0 0 0 1.518-.504 16 16 0 0 0 1.46-.652 16 16 0 0 0 .526-.301l-10.37-10.371A1.993 1.993 0 0 0 12 19c-1.1 0-2 .9-2 2 0 .55.225 1.05.588 1.412l9.13 9.131z" color="#000" font-family="sans-serif" font-weight="400" transform="translate(0 1020.362)"></path><path style="line-height:normal;text-indent:0;text-align:start;text-decoration-line:none;text-decoration-style:solid;text-decoration-color:#000;text-transform:none;block-progression:tb;isolation:auto;mix-blend-mode:normal" fill="#fff" d="M.541 0A.534.534 0 0 0 0 .543a.534.534 0 0 0 .541.525c7.954 0 14.39 6.437 14.39 14.391a.534.534 0 0 0 1.069 0C16 6.928 9.072 0 .541 0Zm.012 4A.546.546 0 0 0 0 4.555a.546.546 0 0 0 .553.537c5.725 0 10.355 4.63 10.355 10.355a.546.546 0 0 0 1.092 0C12 9.132 6.868 4 .553 4ZM.58 8.002a.572.572 0 0 0-.58.58.572.572 0 0 0 .58.563 6.266 6.266 0 0 1 6.275 6.275.572.572 0 0 0 1.145 0C8 11.329 4.671 8.002.58 8.002ZM2 12c-1.1 0-2 .9-2 2s.9 2 2 2 2-.9 2-2-.9-2-2-2zm0 .8c.667 0 1.2.533 1.2 1.2 0 .668-.533 1.201-1.2 1.201-.667 0-1.2-.534-1.2-1.201s.533-1.2 1.2-1.2Z" color="#000" font-family="sans-serif" font-weight="400" overflow="visible" transform="translate(10 1027.362)"></path></g></svg>)rawliteral";
  provisioner.THEME_COLOR = "green";
  provisioner.HTML_TITLE = "Tech Garden Wifi";
  provisioner.PROJECT_TITLE = "Tech Garden Wifi Provision";
  provisioner.PROJECT_INFO = "Please Select Your Connection";
  provisioner.INPUT_LENGTH = "7";
  provisioner.FOOTER_INFO = "Developed by PT Otomasi Cerdas Nusantara";
  provisioner.INPUT_INVALID_LENGTH = "Invalid input length";
  provisioner.INPUT_NOT_VALID = "Invalid Verification Code Please Try Again!";
  provisioner.AP_NAME = "wifi-" + uniqId;
  provisioner.CONNECTION_SUCCESSFUL =
    "Device Will Now Restart"
    "Connection Succsessfull";
  provisioner.setRestartOnSuccess(true);
  provisioner.connectToWiFi();
  // ds3231.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //   if (!SPIFFS.begin(true)) {
  //   Serial.println("SPIFFS Mount Failed");
  //   return;
  // }
  // wifiInit();
  lcd.backlight();
  client.setServer(mqtt_server, 1883);
  client.setCallback(mqttCallback);
}

void wifiInit() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void pinModeAll() {
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);
  pinMode(soilMoisture, INPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(irigasiPin1, OUTPUT);
  pinMode(irigasiPin2, OUTPUT);
  pinMode(mistMakerPin, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
}


void getFileFromServer() {
  WiFiClientSecure client;
  client.setInsecure();  // Set client to allow insecure connections

  if (client.connect(HOST, PORT)) {  // Connect to the server
    Serial.println("Connected to server");
    client.print("GET " + String(PATH) + " HTTP/1.1\r\n");  // Send HTTP GET request
    client.print("Host: " + String(HOST) + "\r\n");         // Specify the host
    client.println("Connection: close\r\n");                // Close connection after response
    client.println();                                       // Send an empty line to indicate end of request headers

    File file = SPIFFS.open("/" + String(FILE_NAME), FILE_WRITE);  // Open file in SPIFFS for writing
    if (!file) {
      Serial.println("Failed to open file for writing");
      return;
    }

    bool endOfHeaders = false;
    String headers = "";
    String http_response_code = "error";
    const size_t bufferSize = 1024;  // Buffer size for reading data
    uint8_t buffer[bufferSize];

    // Loop to read HTTP response headers
    while (client.connected() && !endOfHeaders) {
      if (client.available()) {
        char c = client.read();
        headers += c;
        // Serial.print(c);
        if (headers.startsWith("HTTP/1.1")) {
          http_response_code = headers.substring(9, 12);
        }
        if (headers.endsWith("\r\n\r\n")) {  // Check for end of headers
          endOfHeaders = true;
        }
      }
    }

    // Serial.println("HTTP response code: " + http_response_code);  // Print received headers

    // Loop to read and write raw data to file
    while (client.connected()) {
      if (client.available()) {
        size_t bytesRead = client.readBytes(buffer, bufferSize);
        file.write(buffer, bytesRead);  // Write data to file
      }
    }
    Serial.print("File Size :");
    Serial.println(file.size());
    file.close();   // Close the file
    client.stop();  // Close the client connection
    Serial.println("File saved successfully");
  } else {
    Serial.println("Failed to connect to server");
  }
}

void performOTAUpdateFromSPIFFS() {
  // Open the firmware file in SPIFFS for reading
  File file = SPIFFS.open("/" + String(FILE_NAME), FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.println("Starting update..");
  size_t fileSize = file.size();  // Get the file size
  Serial.println(fileSize);

  // Begin OTA update process with specified size and flash destination
  if (!Update.begin(fileSize, U_FLASH)) {
    Serial.println("Cannot do the update");
    return;
  }

  // Write firmware data from file to OTA update
  Update.writeStream(file);

  // Complete the OTA update process
  if (Update.end()) {
    Serial.println("Successful update");
  } else {
    Serial.println("Error Occurred:" + String(Update.getError()));
    return;
  }

  file.close();  // Close the file
  if (Update.hasError() == true) {
    getFileFromServer();
    performOTAUpdateFromSPIFFS();
  } else {
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("[Update  Firmware]");
    lcd.setCursor(3, 1);
    lcd.print("Update Success");
    lcd.setCursor(3, 2);
    lcd.print("Reset in 5 Sec");
    delay(5000);
    ESP.restart();  // Restart ESP32 to apply the update
  }
}

void irigasiOutput(int x) {
  digitalWrite(in3, x);
  digitalWrite(irigasiPin1, x);
}

void mistMakerOutput(int y) {
  digitalWrite(in1, y);
  digitalWrite(mistMakerPin, y);
}
void wifiIndicatorOutput(int z) {
  digitalWrite(in2, z);
}


void lcdMenu() {
menuAwal:
  while (1) {
    if (!client.connected()) {
      reconnect();
    }
    client.loop();
    readWaterLevel();
    readDht();
    readSoilMoisture();
    DateTime now = ds3231.now();
    lcd.setCursor(14, 0);
    lcd.print(":");
    lcd.setCursor(17, 0);
    lcd.print(":");
    if (now.hour() == 9 && now.minute() <= 10 || now.hour() == 4 && now.minute() <= 10) {
      modeIT = true;
      modeMT = true;
    } else {
      modeIT = false;
      modeMT = false;
    }
    if (temp >= 33) {
      modeMS = true;
    } else {
      modeMS = false;
    }
    if (temp >= 33.5) {
      modeIS = true;
    } else {
      modeIS = false;
    }
    if (now.hour() < 10) {
      lcd.setCursor(12, 0);
      lcd.print("0");
      lcd.setCursor(13, 0);
      lcd.print(now.hour(), DEC);
    } else {
      lcd.setCursor(12, 0);
      lcd.print(now.hour(), DEC);
    }
    if (now.minute() < 10) {
      lcd.setCursor(15, 0);
      lcd.print("0");
      lcd.setCursor(16, 0);
      lcd.print(now.minute(), DEC);
    } else {
      lcd.setCursor(15, 0);
      lcd.print(now.minute(), DEC);
    }
    if (now.second() < 10) {
      lcd.setCursor(18, 0);
      lcd.print("0");
      lcd.setCursor(19, 0);
      lcd.print(now.second(), DEC);
    } else {
      lcd.setCursor(18, 0);
      lcd.print(now.second(), DEC);
    }
    // int temp, hum, wtrlvl, soil, capacity;
    // Serial.print("Temp :");
    // Serial.println(temp);
    // Serial.print("Hum :");
    // Serial.println(hum);
    // Serial.print("Soil :");
    // Serial.println(soil);
    // Serial.print("Water Level :");
    // Serial.println(wtrlvl);

    if (temp >= tempFrom && temp <= tempTo) {
      lcd.setCursor(2, 0);
      lcd.write(6);
    } else {
      lcd.setCursor(2, 0);
      lcd.print(" ");
    }
    if (hum >= humFrom && hum <= humTo) {
      lcd.setCursor(4, 0);
      lcd.write(7);
    } else {
      lcd.setCursor(4, 0);
      lcd.print(" ");
    }

    // lcd.setCursor(0, 1);
    // lcd.print("T:");
    // lcd.print(temp);
    // lcd.print("");
    // lcd.setCursor(0, 2);
    // lcd.print("H:");
    // lcd.print(hum);

    lcd.setCursor(0, 1);
    lcd.print("T:");
    if (temp < 100) {
      lcd.print(temp);
      lcd.setCursor(7, 1);
      lcd.print(" ");
    } else {
      lcd.print(temp);
    }
    lcd.setCursor(0, 2);
    lcd.print("H:");
    if (hum < 100) {
      lcd.print(hum);
      lcd.setCursor(7, 2);
      lcd.print(" ");
    } else {
      lcd.print(hum);
    }


    lcd.setCursor(8, 1);
    lcd.print("C   SM:");
    lcd.setCursor(8, 2);
    lcd.print("Rh  WL:");

    //Soil And Water
    // lcd.setCursor(15, 1);
    // lcd.print(soil);
    // lcd.setCursor(15, 2);
    // lcd.print(wtrlvl);

    lcd.setCursor(15, 1);
    if (soil < 10) {
      lcd.print(soil);
      lcd.print("   ");
    } else if (soil < 100) {
      lcd.print(soil);
      lcd.print("  ");
    } else {
      lcd.print(soil);
    }
    lcd.setCursor(15, 2);
    if (wtrlvl < 10) {
      lcd.print(wtrlvl);
      lcd.print("   ");
    } else if (wtrlvl < 100) {
      lcd.print(wtrlvl);
      lcd.print("  ");
    } else {
      lcd.print(wtrlvl);
    }
    //Persen
    lcd.setCursor(19, 1);
    lcd.print("%");
    lcd.setCursor(19, 2);
    lcd.print("%");

    lcd.setCursor(14, 3);
    lcd.print("[MENU]");
    lcd.setCursor(0, 3);
    //M : Manual (Tombol)
    //S : Sensor
    //A : Aplikasi
    //T : Timer
    lcd.print("I:");
    if (modeIM == true) {
      lcd.setCursor(2, 3);
      lcd.print("M");
    } else {
      lcd.setCursor(2, 3);
      lcd.print(" ");
    }
    if (modeIS == true) {
      lcd.setCursor(3, 3);
      lcd.print("S");
    } else {
      lcd.setCursor(3, 3);
      lcd.print(" ");
    }
    if (modeIA == true) {
      lcd.setCursor(4, 3);
      lcd.print("A");
    } else {
      lcd.setCursor(4, 3);
      lcd.print(" ");
    }
    if (modeIT == true) {
      lcd.setCursor(5, 3);
      lcd.print("T");
    } else {
      lcd.setCursor(5, 3);
      lcd.print(" ");
    }

    lcd.setCursor(7, 3);
    lcd.print("M:");
    if (modeMM == true) {
      lcd.setCursor(9, 3);
      lcd.print("M");
    } else {
      lcd.setCursor(9, 3);
      lcd.print(" ");
    }
    if (modeMS == true) {
      lcd.setCursor(10, 3);
      lcd.print("S");
    } else {
      lcd.setCursor(10, 3);
      lcd.print(" ");
    }
    if (modeMA == true) {
      lcd.setCursor(11, 3);
      lcd.print("A");
    } else {
      lcd.setCursor(11, 3);
      lcd.print(" ");
    }
    if (modeMT == true) {
      lcd.setCursor(12, 3);
      lcd.print("T");
    } else {
      lcd.setCursor(12, 3);
      lcd.print(" ");
    }
    if (modeIM == true || modeIA == true || modeIS == true || modeIT == true) {
      irigasiOutput(1);
    } else if (modeIM == false || modeIA == false || modeIS == false || modeIT == false) {
      irigasiOutput(0);
    }
    if (modeMM == true || modeMA == true || modeMS == true || modeMT == true) {
      mistMakerOutput(1);
    } else if (modeMM == false || modeMA == false || modeMS == false || modeMT == false) {
      mistMakerOutput(0);
    }


    lcd.setCursor(0, 0);
    if (WiFi.status() != WL_CONNECTED) {
      lcd.print("x");
      wifiIndicatorOutput(0);
    } else {
      wifiIndicatorOutput(1);
      if (WiFi.RSSI() > -55) {
        //good Wifi
        lcd.write(2);
      } else if (WiFi.RSSI() > -70) {
        lcd.write(1);
      } else {
        lcd.write(0);
      }
    }
    // lcd.print(WiFi.RSSI());

    // if (millis() - erasePrevious >= erasePeriod) {
    //   lcd.setCursor(2, 1);
    //   lcd.print("      ");
    //   lcd.setCursor(2, 2);
    //   lcd.print("      ");
    //   lcd.setCursor(15, 1);
    //   lcd.print("    ");
    //   lcd.setCursor(15, 2);
    //   lcd.print("    ");
    //   lcd.setCursor(0, 0);
    //   lcd.print(" ");
    //   erasePrevious = millis();
    // }


    if (digitalRead(b1) == LOW) {
      if (btIMLock == true) {
        btIMLock = false;
      } else {
        btIMLock = true;
      }
    }

    if (btIMLock == true) {
      modeIM = true;
    } else {
      modeIM = false;
    }

    if (digitalRead(b2) == LOW) {
      if (btMMLock == true) {
        btMMLock = false;
      } else {
        btMMLock = true;
      }
    }

    if (btMMLock == true) {
      modeMM = true;
    } else {
      modeMM = false;
    }
    if (digitalRead(b3) == LOW) {
      delay(buttonDelay);
      lcd.clear();
      goto menuUtama;
    }
  }

menuUtama:
  while (1) {
    lcd.setCursor(7, 0);
    lcd.print("[MENU]");
    lcd.setCursor(0, 1);
    lcd.print("> Tampilan Awal");
    lcd.setCursor(0, 2);
    lcd.print("  Sync Time");
    lcd.setCursor(0, 3);
    lcd.write(3);
    lcd.print(" UP ");
    lcd.write(4);
    lcd.print(" DN ");
    lcd.write(5);
    lcd.print(" OK ");
    if (digitalRead(b1) == LOW) {
    }
    if (digitalRead(b2) == LOW) {
      delay(buttonDelay);
      lcd.clear();

      goto menuUtama2;
    }
    if (digitalRead(b3) == LOW) {
      delay(buttonDelay);
      lcd.clear();

      goto menuAwal;
    }
  }
menuUtama2:
  while (1) {
    lcd.setCursor(7, 0);
    lcd.print("[MENU]");
    lcd.setCursor(0, 1);
    lcd.print("  Tampilan Awal");
    lcd.setCursor(0, 2);
    lcd.print("> Sync Time");
    lcd.setCursor(0, 3);
    lcd.write(3);
    lcd.print(" UP ");
    lcd.write(4);
    lcd.print(" DN ");
    lcd.write(5);
    lcd.print(" OK ");
    if (digitalRead(b1) == LOW) {
      delay(buttonDelay);
      lcd.clear();

      goto menuUtama;
    }
    if (digitalRead(b2) == LOW) {
      delay(buttonDelay);
      lcd.clear();

      goto menuUtama3;
    }
    if (digitalRead(b3) == LOW) {
      delay(buttonDelay);
      lcd.clear();
      timeClient.begin();
      timeClient.update();
      goto menuTime1;
    }
  }
menuUtama3:
  while (1) {
    lcd.setCursor(7, 0);
    lcd.print("[MENU]");
    lcd.setCursor(0, 1);
    lcd.print("> Wifi Config");
    lcd.setCursor(0, 2);
    lcd.print("  Firmware Update");
    lcd.setCursor(0, 3);
    lcd.write(3);
    lcd.print(" UP ");
    lcd.write(4);
    lcd.print(" DN ");
    lcd.write(5);
    lcd.print(" OK ");
    if (digitalRead(b1) == LOW) {
      delay(buttonDelay);
      lcd.clear();
      goto menuUtama2;
    }
    if (digitalRead(b2) == LOW) {
      delay(buttonDelay);
      lcd.clear();

      goto menuUtama4;
    }
    if (digitalRead(b3) == LOW) {
      delay(buttonDelay);
      lcd.clear();


      goto menuWifi1;
    }
  }

menuUtama4:
  while (1) {
    lcd.setCursor(7, 0);
    lcd.print("[MENU]");
    lcd.setCursor(0, 1);
    lcd.print("  Wifi Config");
    lcd.setCursor(0, 2);
    lcd.print("> Firmware Update");
    lcd.setCursor(0, 3);
    lcd.write(3);
    lcd.print(" UP ");
    lcd.write(4);
    lcd.print(" DN ");
    lcd.write(5);
    lcd.print(" OK ");
    if (digitalRead(b1) == LOW) {
      delay(buttonDelay);
      lcd.clear();

      goto menuUtama3;
    }
    if (digitalRead(b2) == LOW) {
    }
    if (digitalRead(b3) == LOW) {
      delay(buttonDelay);
      lcd.clear();
      pilihan = 0;
      goto menuFirmware1;
    }
  }

menuTime1:
  while (1) {
    lcd.setCursor(4, 0);
    lcd.print("[Sync  Time]");
    lcd.setCursor(0, 1);
    lcd.print("Dev Time:");
    DateTime now = ds3231.now();
    lcd.setCursor(12, 1);
    lcd.print(":");
    lcd.setCursor(15, 1);
    lcd.print(":");
    if (now.hour() < 10) {
      lcd.setCursor(10, 1);
      lcd.print("0");
      lcd.setCursor(11, 1);
      lcd.print(now.hour(), DEC);
    } else {
      lcd.setCursor(10, 1);
      lcd.print(now.hour(), DEC);
    }
    if (now.minute() < 10) {
      lcd.setCursor(13, 1);
      lcd.print("0");
      lcd.setCursor(14, 1);
      lcd.print(now.minute(), DEC);
    } else {
      lcd.setCursor(13, 1);
      lcd.print(now.minute(), DEC);
    }
    if (now.second() < 10) {
      lcd.setCursor(16, 1);
      lcd.print("0");
      lcd.setCursor(17, 1);
      lcd.print(now.second(), DEC);
    } else {
      lcd.setCursor(16, 1);
      lcd.print(now.second(), DEC);
    }
    lcd.setCursor(0, 2);
    lcd.print("Int Time:");
    lcd.setCursor(12, 2);
    lcd.print(":");
    lcd.setCursor(15, 2);
    lcd.print(":");
    if (timeClient.getHours() < 10) {
      lcd.setCursor(10, 2);
      lcd.print("0");
      lcd.setCursor(11, 2);
      lcd.print(timeClient.getHours(), DEC);
    } else {
      lcd.setCursor(10, 2);
      lcd.print(timeClient.getHours(), DEC);
    }
    if (timeClient.getMinutes() < 10) {
      lcd.setCursor(13, 2);
      lcd.print("0");
      lcd.setCursor(14, 2);
      lcd.print(timeClient.getMinutes(), DEC);
    } else {
      lcd.setCursor(13, 2);
      lcd.print(timeClient.getMinutes(), DEC);
    }
    if (timeClient.getSeconds() < 10) {
      lcd.setCursor(16, 2);
      lcd.print("0");
      lcd.setCursor(17, 2);
      lcd.print(timeClient.getSeconds(), DEC);
    } else {
      lcd.setCursor(16, 2);
      lcd.print(timeClient.getSeconds(), DEC);
    }
    lcd.setCursor(0, 3);
    lcd.print("Sync now ?");
    lcd.setCursor(12, 3);
    lcd.print("YES");
    lcd.setCursor(17, 3);
    lcd.print("NO");

    if (pilihan == 0) {
      lcd.setCursor(11, 3);
      lcd.print(" ");
      lcd.setCursor(16, 3);
      lcd.print(">");
    } else if (pilihan == 1) {
      lcd.setCursor(11, 3);
      lcd.print(">");
      lcd.setCursor(16, 3);
      lcd.print(" ");
    }
    if (digitalRead(b1) == LOW) {
      pilihan = 0;
    }
    if (digitalRead(b2) == LOW) {
      pilihan = 1;
    }
    if (digitalRead(b3) == LOW) {
      if (pilihan == 1) {
        ds3231.adjust(DateTime(timeClient.getEpochTime()));
        delay(buttonDelay);
        lcd.clear();
        goto menuUtama;
      } else if (pilihan == 0) {
        delay(buttonDelay);
        lcd.clear();
        goto menuUtama;
      }
    }
  }


menuWifi1:
  while (1) {
    lcd.setCursor(4, 0);
    lcd.print("[Wifi Config]");
    lcd.setCursor(3, 1);
    lcd.print("Want to Update");
    lcd.setCursor(2, 2);
    lcd.print("Wifi Credentials ? ");
    lcd.setCursor(6, 3);
    lcd.print("YES");
    lcd.setCursor(12, 3);
    lcd.print("NO");
    if (pilihan == 0) {
      lcd.setCursor(5, 3);
      lcd.print(" ");
      lcd.setCursor(11, 3);
      lcd.print(">");
    } else if (pilihan == 1) {
      lcd.setCursor(5, 3);
      lcd.print(">");
      lcd.setCursor(11, 3);
      lcd.print(" ");
    }
    if (digitalRead(b1) == LOW) {
      pilihan = 0;
    }
    if (digitalRead(b2) == LOW) {
      pilihan = 1;
    }
    if (digitalRead(b3) == LOW) {
      if (pilihan == 1) {
        delay(buttonDelay);
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("[Wifi  Config]");
        lcd.setCursor(3, 1);
        lcd.print("Device AP NAME");
        lcd.setCursor(0, 2);
        lcd.print("wifi-");
        lcd.setCursor(0, 3);
        lcd.print(uniqId);
        provisioner.resetCredentials();
        provisioner.setupAccessPointAndServer();


        // goto menuWifi2;
      } else if (pilihan == 0) {
        delay(buttonDelay);

        lcd.clear();
        goto menuUtama;
      }
    }
  }
  // menuWifi2:
  //   while (1) {
  //     lcd.setCursor(4, 0);
  //     lcd.print("[Wifi Config]");
  //     lcd.setCursor(3, 1);
  //     lcd.print("Device AP NAME");
  //     lcd.setCursor(0, 2);
  //     lcd.print("wifi");
  //     lcd.print(uniqId);
  //   }

menuFirmware1:
  while (1) {
    lcd.setCursor(1, 0);
    lcd.print("[Update  Firmware]");
    lcd.setCursor(3, 1);
    lcd.print("Want to Update");
    lcd.setCursor(5, 2);
    lcd.print("Firmware ? ");
    lcd.setCursor(6, 3);
    lcd.print("YES");
    lcd.setCursor(12, 3);
    lcd.print("NO");
    if (pilihan == 0) {
      lcd.setCursor(5, 3);
      lcd.print(" ");
      lcd.setCursor(11, 3);
      lcd.print(">");
    } else if (pilihan == 1) {
      lcd.setCursor(5, 3);
      lcd.print(">");
      lcd.setCursor(11, 3);
      lcd.print(" ");
    }
    if (digitalRead(b1) == LOW) {
      pilihan = 0;
    }
    if (digitalRead(b2) == LOW) {
      pilihan = 1;
    }
    if (digitalRead(b3) == LOW) {
      if (pilihan == 1) {
        delay(buttonDelay);
        lcd.clear();
        goto menuFirmware2;
      } else if (pilihan == 0) {
        delay(buttonDelay);
        lcd.clear();
        goto menuUtama;
      }
    }
  }

menuFirmware2:
  while (1) {
    lcd.setCursor(1, 0);
    lcd.print("[Update  Firmware]");
    lcd.setCursor(4, 1);
    lcd.print("Please  Wait");
    lcd.setCursor(loading, 2);
    lcd.print(".");
    if (millis() - loadingPrevious >= 500) {
      loading++;
      loadingPrevious = millis();
    }
    if (loading >= 20) {
      lcd.clear();
      loading = 0;
    };
    if (!SPIFFS.begin(true)) {
      Serial.println("SPIFFS Mount Failed");
      return;
    }
    getFileFromServer();
    performOTAUpdateFromSPIFFS();
  }
}

 
#include "CTBot.h" //Pendeklarasian Library
#include <ArduinoJson.h>

CTBot myBot;
TBMessage msg;

String ssid = "PLAY MEDIA";    //nama ssid wifi kalian
String pass = "11223344";  //password wifi kalian
String token = "5329328492:AAFNdwVbiZ4O3NX-t8eyar9mkS1fxR58sGY";    //token bot baru kalian
const int id = 834574145;      //id telegram kalian

void setup() { //Pengaturan Variabel
  Serial.begin(9600);
  Serial.println("Starting TelegramBot...");
  myBot.wifiConnect(ssid, pass);
  myBot.setTelegramToken(token);
  if (myBot.testConnection()) {
    Serial.println("Koneksi Bagus");
  } else {
    Serial.println("Koneksi Jelek");
  }
    myBot.sendMessage(msg.sender.id, (String)"Halo Sahabat Indobot");
}

void loop() {}  //Perulangan Program

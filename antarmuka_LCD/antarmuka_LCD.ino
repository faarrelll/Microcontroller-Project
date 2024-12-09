#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTTYPE DHT22

#define DHTPIN1 4  //kanan   
#define DHTPIN2 16 // atas
#define DHTPIN3 33 // kiri

#define bt_kanan 13 // merah
#define bt_tengah 14 // kuning
#define bt_kiri 27 // biru

int pilih = 0;
int menu = 0;

LiquidCrystal_I2C lcd(0x27, 20, 4);
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);

const int Fan = 23; //inhouse
const int Fan2 = 25; // fan heater 
const int Fan3 = 26; // exhouse

const int freq = 20000;
const int ledChannel = 0;
const int ledChannel2 = 1;
const int ledChannel3 = 2;
const int resolution = 8;

int bst_lpwm = 18;
int bst_rpwm = 19;  
int channel = 4;
int channel2 = 5;
int freq2  = 1000;

void setup() {
  lcd.begin(); // initialize the lcd
  lcd.backlight();
  dht1.begin();
  dht2.begin();
  dht3.begin();

  ledcSetup(ledChannel, freq, resolution);
  ledcSetup(ledChannel2, freq, resolution);
  ledcSetup(ledChannel3, freq, resolution);

  ledcSetup(channel, freq2, resolution);
  ledcSetup(channel2, freq2, resolution);
  
  ledcAttachPin(bst_lpwm,channel);
  ledcAttachPin(bst_rpwm,channel2);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(Fan, ledChannel);
  ledcAttachPin(Fan2, ledChannel2);
  ledcAttachPin(Fan3, ledChannel3);
  
  pinMode(bt_kanan, INPUT_PULLUP);
  pinMode(bt_tengah, INPUT_PULLUP);
  pinMode(bt_kiri, INPUT_PULLUP);
  loading();
}

void loop() {
  int btn_menu = digitalRead(bt_kiri);
  int btn_pilih = digitalRead(bt_tengah);
  int btn_batal = digitalRead(bt_kanan);

  antarmuka();

  if (btn_menu == LOW) {
    hapus_antarmuka();
    if (pilih == 0 || pilih == 1) {
      if (menu == 2) {
        menu = 1;
      } else {
        menu = menu + 1;
      }
    }
  } else if (btn_pilih == LOW) {
    hapus_antarmuka();
    
    if (menu == 1 && pilih == 1){
      pilih = 3;
      }
    else if (menu == 2 && pilih == 1){
      pilih = 4;
      }
    else if (menu == 1 && pilih == 3){
      menu = menu;
    }
     else if(menu == 2 && pilih == 4){
      
        menu = menu;
    } 
    else if (menu == 1) {
      menu = 0;
      pilih = 1;
    } else if (menu == 2) {
      pilih = 2;
    }
  } else if (btn_batal == LOW) {
    hapus_antarmuka();
    if(pilih == 3 || pilih ==4){
      pilih = 1;
      ledcWrite(ledChannel, 0);
      ledcWrite(ledChannel2, 0);
      ledcWrite(ledChannel3, 0);
      ledcWrite(channel, 0);
      ledcWrite(channel2,0);
     } else {
      pilih = 0;
     }
    
  }
}

void antarmuka() {
  if (pilih == 0) {
    if (menu == 0) {
      lcd.setCursor(8, 0);
      lcd.print("MENU");
      lcd.setCursor(0, 1);
      lcd.print("  Pengerigan Sepatu");
      lcd.setCursor(0, 2);
      lcd.print("  Cek Parameter");
    } else if (menu == 1) {
      lcd.setCursor(8, 0);
      lcd.print("MENU");
      lcd.setCursor(0, 1);
      lcd.print("> Pengerigan Sepatu");
      lcd.setCursor(0, 2);
      lcd.print("  Cek Parameter");
    } else if (menu == 2) {
      lcd.setCursor(8, 0);
      lcd.print("MENU");
      lcd.setCursor(0, 1);
      lcd.print("  Pengerigan Sepatu");
      lcd.setCursor(0, 2);
      lcd.print("> Cek Parameter");
    }
  } else if (pilih == 1) {
    if(menu == 0){
      lcd.setCursor(4, 0);
      lcd.print("PENGERINGAN");
      lcd.setCursor(0, 1);
      lcd.print("  Sepatu Casual");
      lcd.setCursor(0, 2);
      lcd.print("  Sepatu Running");
    }
    else if(menu == 1){
      lcd.setCursor(4, 0);
      lcd.print("PENGERINGAN");
      lcd.setCursor(0, 1);
      lcd.print("> Sepatu Casual");
      lcd.setCursor(0, 2);
      lcd.print("  Sepatu Running");
      
    }
    else if(menu == 2){
      lcd.setCursor(4, 0);
      lcd.print("PENGERINGAN");
      lcd.setCursor(0, 1);
      lcd.print("  Sepatu Casual");
      lcd.setCursor(0, 2);
      lcd.print("> Sepatu Running");
      
    }
    
  } 
  else if (pilih == 2) {
    lcd.setCursor(5, 0);
    lcd.print("PARAMETER");
    suhu();
  } 
  else if (pilih == 3){
    lcd.setCursor(1,0);
    lcd.print("PENGERINGAN SEPATU");
    lcd.setCursor(0,1);
    lcd.print("Jenis      = Casual");
    lcd.setCursor(0, 2);
    lcd.print("Suhu       = 29 C");
    lcd.setCursor(0, 3);
    lcd.print("Kelembapan = 73 %");
    ledcWrite(ledChannel, 255);
    ledcWrite(ledChannel2, 255);
    ledcWrite(ledChannel3, 255);
    ledcWrite(channel, 255);
    ledcWrite(channel2,0);
    }
   else if (pilih == 4){
    lcd.setCursor(1,0);
    lcd.print("PENGERINGAN SEPATU");
    lcd.setCursor(0,1);
    lcd.print("Jenis      = Running");
    lcd.setCursor(0, 2);
    lcd.print("Suhu       = 29 C");
    lcd.setCursor(0, 3);
    lcd.print("Kelembapan = 73 %");
    ledcWrite(ledChannel, 150);
    ledcWrite(ledChannel2, 150);
    ledcWrite(ledChannel3, 150);
    ledcWrite(channel, 255);
    ledcWrite(channel2,0);
    } 
}

void loading() {
  lcd.setCursor(2, 1);
  lcd.print("System Loading..");
  for (int i = 0; i < 20; i++) {
    lcd.setCursor(i, 2);
    lcd.print("*");
    delay(100); // Atur kecepatan loading
  }
  delay(1000);

  lcd.setCursor(0, 2);
  for (int i = 0; i < 20; i++) {
    lcd.print(" ");
  }
  lcd.setCursor(0, 1);
  for (int i = 0; i < 20; i++) {
    lcd.print(" ");
  }

  // Tampilkan pesan loading selesai
  lcd.setCursor(1, 1);
  lcd.print("Loading Complete!!");

  // Tunggu sebentar setelah pesan loading selesai ditampilkan
  delay(3000);

  // Hapus pesan loading selesai
  lcd.setCursor(0, 1);
  for (int i = 0; i < 20; i++) {
    lcd.print(" ");
  }
}

void hapus_antarmuka() {
  // hapus tampilan
  lcd.setCursor(0, 0);
  for (int a = 0; a < 20; a++) {
    lcd.print(" ");
  }
  lcd.setCursor(0, 1);
  for (int a = 0; a < 20; a++) {
    lcd.print(" ");
  }
  lcd.setCursor(0, 2);
  for (int a = 0; a < 20; a++) {
    lcd.print(" ");
  }
  lcd.setCursor(0, 3);
  for (int a = 0; a < 20; a++) {
    lcd.print(" ");
  }
}

void suhu(){
  
  float humi1  = dht1.readHumidity();    // read humidity
  float tempC1 = dht1.readTemperature(); // read temperature
  float humi2  = dht2.readHumidity();    // read humidity
  float tempC2 = dht2.readTemperature(); // read temperature
  float humi3  = dht3.readHumidity();    // read humidity
  float tempC3 = dht3.readTemperature(); // read temperature

  
  if (isnan(tempC1) || isnan(humi1)) {
    lcd.setCursor(0, 1);
    lcd.print("Failed");
  } else {
    lcd.setCursor(0, 1);  // display position
    lcd.print("S:");
    lcd.print(tempC1);     // display the temperature
    lcd.print("C");
    lcd.print(" ");
   // display position
    lcd.print("K:");
    lcd.print(humi1);      // display the humidity
    lcd.print("%");
  }

  if (isnan(tempC2) || isnan(humi2)) {
    lcd.setCursor(0, 2);
    lcd.print("Failed");
  } else {
    lcd.setCursor(0, 2);  // display position
    lcd.print("S:");
    lcd.print(tempC2);     // display the temperature
    lcd.print("C");
    lcd.print(" ");
  // display position
    lcd.print("K:");
    lcd.print(humi2);      // display the humidity
    lcd.print("%");
  }

  if (isnan(tempC3) || isnan(humi3)) {
    lcd.setCursor(0, 3);
    lcd.print("Failed");
  } else {
    lcd.setCursor(0, 3);  // display position
    lcd.print("S:");
    lcd.print(tempC3);     // display the temperature
    lcd.print("C");
    lcd.print(" ");
      // display position 
    lcd.print("K:");
    lcd.print(humi3);      // display the humidity
    lcd.print("%");
  }
  }

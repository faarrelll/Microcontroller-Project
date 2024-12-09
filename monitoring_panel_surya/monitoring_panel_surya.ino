#include <RTClib.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_INA219.h>
#include <SD.h>

Adafruit_INA219 ina219;
LiquidCrystal_I2C lcd(0x27, 16, 2);

float busvoltage = 0;
float current_mA = 0;
float power_mW = 0;

char daysOfWeek[7][12] = {
  "Sunday",
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday"
};

RTC_DS3231 rtc;
const int chipSelect = 4; // Pin untuk microSD

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Monitoring");
  lcd.setCursor(0, 1);
  lcd.print("Panel Surya");
  delay(2000);

  if (! ina219.begin()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FAILED TO FIND");
    lcd.setCursor(0, 1);
    lcd.print(" INA219 MODULE");
    while (1) {
      delay(10);
    }
  }

  if (!rtc.begin()) {
    Serial.println("RTC module is NOT found");
    Serial.flush();
    while (1);
  }

  // Inisialisasi microSD
  if (!SD.begin(chipSelect)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FAILED TO FIND");
    lcd.setCursor(0, 1);
    lcd.print(" SD CARD");
    while (1);
  }
}

void loop() {
  pengukuran();
}

String waktu() {
  DateTime now = rtc.now();
  String dateTimeString = String(now.year()) + "/" +
                          String(now.month()) + "/" +
                          String(now.day()) + " (" +
                          String(daysOfWeek[now.dayOfTheWeek()]) + ") " +
                          String(now.hour()) + ":" +
                          String(now.minute()) + ":" +
                          String(now.second());
  return dateTimeString;
}

void pengukuran() {
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  
  // Menyimpan data ke microSD
  DateTime now = rtc.now();
  String dateTimeString = String(now.year()) + "-" +
                          String(now.month()) + "-" +
                          String(now.day()) + " " +
                          String(now.hour()) + ":" +
                          String(now.minute()) + ":" +
                          String(now.second());

  File dataFile = SD.open("data.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print("Time: ");
    dataFile.print(dateTimeString);
    dataFile.print(", Voltage: ");
    dataFile.print(busvoltage);
    dataFile.print(" V, Current: ");
    dataFile.print(current_mA);
    dataFile.print(" mA, Power: ");
    dataFile.close();
  } else {
    Serial.println("Error opening data.txt");
  }

  // Menampilkan data di LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(busvoltage);
  lcd.setCursor(0, 1);
  lcd.print("I:");

  if (current_mA < 3) {
    lcd.print("000");
  } else if (current_mA < 10) {
    lcd.print("00");
    lcd.print((int)current_mA);
  } else if (current_mA < 100) {
    lcd.print('0');
    lcd.print((int)current_mA);
  } else {
    lcd.print((int)current_mA);
  }

  delay(2000);
}

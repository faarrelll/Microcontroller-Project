#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensorSuhu(&oneWire);


LiquidCrystal_I2C lcd(0x27, 16, 2);
int addr = 0;
int val;
int nilai_suhu;


void setup() {
  
  Serial.begin(9600);
  lcd.begin();
  lcd.clear();
  val = EEPROM.read(addr);
  Serial.print("Suhu Sebelumnya : ");
  Serial.println(val);
}

void loop() {
  
  nilai_suhu = baca_suhu();
  Serial.print("Suhu saat ini: ");
  Serial.print(nilai_suhu);
  Serial.println("Celcius");


  lcd.setCursor(0, 0);
  lcd.print("Suhu: ");
  lcd.print(nilai_suhu);
  lcd.print(" C");

  EEPROM.write(addr,nilai_suhu);

  delay(1000);
}


float baca_suhu(){

  sensorSuhu.requestTemperatures();
  float suhu = sensorSuhu.getTempCByIndex(0);
  return suhu; 
  }

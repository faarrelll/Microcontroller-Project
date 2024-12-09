#include <OneWire.h>

#include <DallasTemperature.h>

#define ONE_WIRE_BUS 8

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensorSuhu(&oneWire);

float nilai_suhu;

void setup() {

  // put your setup code here, to run once:

  Serial.begin(9600);

}

float baca_suhu(){

  sensorSuhu.requestTemperatures();

  float suhu = sensorSuhu.getTempCByIndex(0);

  return suhu; }

void loop() {

  // put your main code here, to run repeatedly:

  nilai_suhu=baca_suhu();

  Serial.print(nilai_suhu);

  Serial.println("Celcius");

}

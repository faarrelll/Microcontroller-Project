#include <Wire.h>
#include <Servo.h>
#include <EEPROM.h>
#include <DHT.h>
#include <DS3231.h>

#define DHTPIN 2 // Pin data untuk sensor DHT11
#define DHTTYPE DHT11 // Jenis sensor DHT11
DHT dht(DHTPIN, DHTTYPE);

#define SERVOPIN 3 // Pin untuk mengontrol motor servo
Servo myservo;

DS3231 rtc(SDA, SCL);

void setup() {
  Serial.begin(9600);
  dht.begin();
  myservo.attach(SERVOPIN);

  Wire.begin();
  rtc.begin();

  // Inisialisasi EEPROM
  EEPROM.begin(512);
}

void loop() {
  // Baca data suhu dan kelembaban dari sensor DHT11
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Jika pembacaan sensor DHT11 berhasil
  if (!isnan(h) && !isnan(t)) {
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C");

    // Simpan data suhu dan kelembaban ke EEPROM
    int addr = 0;
    float temp = t;
    EEPROM.put(addr, temp);
    addr += sizeof(temp);
    float hum = h;
    EEPROM.put(addr, hum);
    addr += sizeof(hum);
    EEPROM.commit();
  }

  // Baca waktu dari RTC
  rtc.update();
  DateTime now = rtc.now();

  // Set level air berdasarkan waktu dan suhu
  int hour = now.hour();
  if (hour < 8 || hour > 20) {
    myservo.write(0); // Level air rendah saat malam hari
  } else {
    float temp;
    EEPROM.get(0, temp);
    if (temp > 28) {
      myservo.write(180); // Level air tinggi saat suhu panas
    } else if (temp < 22) {
      myservo.write(90); // Level air sedang saat suhu sejuk
    } else {
      myservo.write(45); // Level air rendah saat suhu normal
    }
  }

  delay(10000); // Jeda 10 detik
}

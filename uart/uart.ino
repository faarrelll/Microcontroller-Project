/*********************************
 * Monitoring Suhu Kelembaban
 * Sensor : DHT11
 * Board  : Arduino UNO
 * Output : Serial (9600)
 * DOUT -> pin 2
*********************************/
#include<SoftwareSerial.h>
#include <DHT.h> //library DHT
#define DHTPIN 2 //pin DATA konek ke pin 2 Arduino
#define DHTTYPE DHT11 //tipe sensor DHT11
DHT dht(DHTPIN, DHTTYPE); //set sensor + koneksi pin

SoftwareSerial Hujan(3, 4);// RX, TX

//Data Tampung
int Data;


int humi, temp;//deklarasi variabel 

void setup()
{
  Serial.begin(9600); //baud 9600
  Hujan.begin(9600);
  delay(10);  
}

void loop()
{
  humi = dht.readHumidity();//baca kelembaban
  temp = dht.readTemperature();//baca suhu
  if (isnan(humi) || isnan(temp)) { //jika tidak ada hasil
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000);                       // wait for a second
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(1000);      
    return;
  }
  else{//jika ada hasilnya 
  Serial.print("Suhu=");  //kirim serial "Suhu"
  Serial.print(temp);     //kirim serial nilai suhu
  Serial.println("C");    //kirim serial "C" Celcius 
  Serial.print("Humi=");  //kirim serial "Humi"
  Serial.print(humi);     //kirim serial nilai kelembaban
  Serial.println("%RH");  //kirim serial "%RH"
  Data = 9;
  Hujan.println(Data);
  }
  delay(1000);            //tunda 1 detik untuk pembacaan berikutnya
}

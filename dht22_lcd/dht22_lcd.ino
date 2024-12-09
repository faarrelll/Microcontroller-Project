
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#define DHTTYPE DHT22


#define DHTPIN1 4     
#define DHTPIN2 16
#define DHTPIN3 33
#define bt1 13
#define bt2 14 
#define bt3 27

LiquidCrystal_I2C lcd(0x27, 20, 4);  // I2C address 0x27 (from DIYables LCD), 16 column and 2 rows
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);

const int Fan = 23;  
const int Fan2 = 25; 
const int Fan3 = 26; 

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
  Serial.begin(115200);
  dht1.begin();
  dht2.begin();
  dht3.begin();// initialize the DHT22 sensor
  lcd.begin();         // initialize the lcd
  lcd.backlight();    // open the backlight

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

  pinMode(bt1, INPUT_PULLUP);
  pinMode(bt2, INPUT_PULLUP);
  pinMode(bt3, INPUT_PULLUP);
  
}

void loop() {
  float humi1  = dht1.readHumidity();    // read humidity
  float tempC1 = dht1.readTemperature(); // read temperature
  float humi2  = dht2.readHumidity();    // read humidity
  float tempC2 = dht2.readTemperature(); // read temperature
  float humi3  = dht3.readHumidity();    // read humidity
  float tempC3 = dht3.readTemperature(); // read temperature

  int val1 = digitalRead(bt1);
  int val2 = digitalRead(bt2);
  int val3 = digitalRead(bt3);
  
//  lcd.clear();

//    lcd.print("     ");
  // check whether the reading is successful or not
  if (isnan(tempC1) || isnan(humi1)) {
    lcd.setCursor(0, 0);
    lcd.print("Failed");
  } else {
    lcd.setCursor(0, 0);  // display position
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
    lcd.setCursor(0, 1);
    lcd.print("Failed");
  } else {
    lcd.setCursor(0, 1);  // display position
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
    lcd.setCursor(0, 2);
    lcd.print("Failed");
  } else {
    lcd.setCursor(0, 2);  // display position
    lcd.print("S:");
    lcd.print(tempC3);     // display the temperature
    lcd.print("C");
    lcd.print(" ");
      // display position 
    lcd.print("K:");
    lcd.print(humi3);      // display the humidity
    lcd.print("%");
  }
//
  lcd.setCursor(0,3);
  lcd.print("b1:");
  lcd.print(val1);
  lcd.print(" b2:");
  lcd.print(val2);
  lcd.print(" b3:");
  lcd.print(val3);

   //kipas
   ledcWrite(ledChannel, 250);
   ledcWrite(ledChannel2, 250);
   ledcWrite(ledChannel3, 250);

   //pemanas
   ledcWrite(channel, 255);
   ledcWrite(channel2,0);
    
  
//  delay(2000);
}

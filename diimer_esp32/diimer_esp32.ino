#include <RBDdimmer.h>//https://github.com/RobotDynOfficial/RBDDimmer

//Parameters
const int zeroCrossPin  = 12;
const int acdPin  = 13;
 
int MIN_POWER  = 0;
int MAX_POWER  = 80;
int POWER_STEP  = 2;

//Variables
int power  = 30;
//Objects
dimmerLamp acd(acdPin,zeroCrossPin);
void setup(){
//Init Serial USB
Serial.begin(115200);
Serial.println(F("ESP32 System"));
acd.begin(NORMAL_MODE, ON);
// Start the DS18B20 sensor

}
void loop(){
  testDimmer();
  delay(1000);
}
void testDimmer(){/* function testDimmer */
////Sweep light power to test dimmer
  
      acd.setPower(power); // setPower(0-100%);
      Serial.print("lampValue -> ");
      Serial.print(acd.getPower());
      Serial.println("%");
      delay(100);
  
}

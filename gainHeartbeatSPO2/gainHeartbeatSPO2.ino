#include "DFRobot_BloodOxygen_S.h"
#define I2C_ADDRESS    0x57


DFRobot_BloodOxygen_S_I2C MAX30102(&Wire ,I2C_ADDRESS);

//DFRobot_BloodOxygen_S_HardWareUart MAX30102(&Serial1, 9600); // if using UART (ESP32)


void setup()
{
  Serial.begin(115200);
  while (false == MAX30102.begin())
  {
    Serial.println("init fail!");
    delay(1000);
  }
  Serial.println("init success!");
  Serial.println("start measuring...");
  MAX30102.sensorStartCollect();
}

void loop()
{
  MAX30102.getHeartbeatSPO2();
  Serial.print("SPO2 is : ");
  Serial.print(MAX30102._sHeartbeatSPO2.SPO2);
  Serial.println("%");
  Serial.print("heart rate is : ");
  Serial.print(MAX30102._sHeartbeatSPO2.Heartbeat);
  Serial.println("Times/min");
  Serial.print("Temperature value of the board is : ");
  Serial.print(MAX30102.getTemperature_C());
  Serial.println(" ℃");
  //The sensor updates the data every 4 seconds
  delay(4000);
  //Serial.println("stop measuring...");
  //MAX30102.sensorEndCollect();
}
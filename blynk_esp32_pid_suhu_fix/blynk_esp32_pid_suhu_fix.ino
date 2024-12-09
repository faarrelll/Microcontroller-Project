#define BLYNK_TEMPLATE_ID "TMPL4xTESxqP"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "ytbfOEHLgcHi6KN6n2bwXEe0m86Rd_Cz"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <RBDdimmer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int zeroCrossPin  = 12;
const int acdPin  = 13;
const int oneWireBus = 4;  
float error, eprev, eprev_2;
float I, prevI;
float U;
double setpoint;
double kp = 30;
double ki;
double kd = 2;
float suhu= 0;
int ts = 20;

OneWire oneWire(oneWireBus);

DallasTemperature sensors(&oneWire);

dimmerLamp acd(acdPin,zeroCrossPin);
char auth[] = BLYNK_AUTH_TOKEN;


char ssid[] = "Kon";
char pass[] = "11223344";
int setvalue;

BlynkTimer timer;

void ssuhu()
{
  
  sensors.requestTemperatures(); 
  suhu = sensors.getTempCByIndex(0);
  Serial.print(suhu);
  Serial.println("ºC");
  Blynk.virtualWrite(V7, suhu);
  delay(1000);
}
BLYNK_WRITE(V4)
{
  setpoint = param.asInt(); // assigning incoming value from pin V1 to a variable
  Serial.println(setpoint);


}
//BLYNK_WRITE(V8)
//{
////  kp = param.asInt(); // assigning incoming value from pin V1 to a variable
////  Serial.println(kp);
//
//
//}
//BLYNK_WRITE(V9)
//{
////  kd = param.asInt(); // assigning incoming value from pin V1 to a variable
////  Serial.println(kd);
//

//}


void setup()
{
  
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L,ssuhu);
  acd.begin(NORMAL_MODE, ON);
  sensors.begin();
  
  
}

void loop()
{
  Blynk.run();
  timer.run();
  error = setpoint - suhu;
   I = prevI + ki*((ts/2)*(error + 2*eprev + eprev));
   U = kp*error + I + kd*(error-eprev);

   if(U > 80){
      U = 80;
    } else if(U < 0){
      U = 0;
      } else {
        U = U;
        }

   prevI = I;
   eprev = error;
   eprev_2 = eprev;

   acd.setPower(U);
   
    
}


  
  


      
  

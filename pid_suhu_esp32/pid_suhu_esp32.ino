#include <RBDdimmer.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const int zeroCrossPin  = 12;
const int acdPin  = 13;
const int oneWireBus = 4;  

OneWire oneWire(oneWireBus);

DallasTemperature sensors(&oneWire);

dimmerLamp acd(acdPin,zeroCrossPin);

float error, eprev, eprev_2;
float I, prevI;
float U;
double setpoint = 30;
double kp = 10;
double ki = 0;
double kd = 2;
float suhu= 0;
int ts = 20;

 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  acd.begin(NORMAL_MODE, ON);
  sensors.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  sensor();
  pid();
}

void sensor(){
  
  sensors.requestTemperatures(); 
  suhu = sensors.getTempCByIndex(0);
  Serial.print(suhu);
  Serial.println("ºC");
  delay(1000);  
}
void pid(){

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

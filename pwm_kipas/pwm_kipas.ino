const int Fan = 13;  // 5 corresponds to GPIO5
const int Fan2 = 19; 
// setting PWM properties
const int freq = 20000;
const int ledChannel = 0;
const int ledChannel2 = 2;
const int resolution = 8;
 
void setup(){
  // configure LED PWM functionalitites
  Serial.begin(9600);
  ledcSetup(ledChannel, freq, resolution);
  ledcSetup(ledChannel2, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(Fan, ledChannel);
  ledcAttachPin(Fan2, ledChannel2);

}
 
void loop(){
//
//    for(int dutyCycle = 50; dutyCycle <= 255; dutyCycle++){
//    //changing the LED brightness with PWM
//    ledcWrite(ledChannel, dutyCycle);
//    Serial.print("pwm :");
//    Serial.println(dutyCycle); 
//    delay(100);
//}
   ledcWrite(ledChannel, 250);
   ledcWrite(ledChannel2, 150);
}

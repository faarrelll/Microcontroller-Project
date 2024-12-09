// the number of the LED pin
#define pwma 18  // 16 corresponds to GPIO16
#define pwmb 5 // 17 corresponds to GPIO17
#define pwmc 17  // 5 corresponds to GPIO5
#define pwmd 16  // 5 corresponds to GPIO5

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

void setup() {
  ledcSetup(0, freq, resolution);
  ledcSetup(1, freq, resolution);
  ledcAttachPin(pwma, 0);
  ledcAttachPin(pwmc, 1);
}

void loop() {
  ledcWrite(0, 100);
  ledcWrite(1, 100);
  digitalWrite(pwmb, LOW);
  digitalWrite(pwmd, LOW);

}

int bst_lpwm = 18;
int bst_rpwm = 19;  
//int En = 14;
int channel = 0;
int channel2 = 1;
int freq  = 1000;
int res = 8;

void setup() {
  // put your setup code here, to run once:
  ledcSetup(channel, freq, res);
  ledcSetup(channel2, freq, res);
  ledcAttachPin(bst_lpwm,channel);
  ledcAttachPin(bst_rpwm,channel2);
//  pinMode(En, OUTPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
//  digitalWrite(En,HIGH);
  ledcWrite(channel, 150-0);
  ledcWrite(channel2,0);
}

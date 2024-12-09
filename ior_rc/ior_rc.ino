#define pwma D1  // 16 corresponds to GPIO16
#define pwmb D2 // 17 corresponds to GPIO17
#define pwmc D3  // 5 corresponds to GPIO5
#define pwmd D4
#define lamp_d D5
#define lamp_b D7
#define riting_kn D8
#define riting_kr 3

bool ld,lb,rkn,rkr;

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(pwma, OUTPUT);
    pinMode(pwmc, OUTPUT);
    pinMode(pwmb, OUTPUT);
    pinMode(pwmd, OUTPUT);
    pinMode(ld, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
//          analogWrite(pwmb, );
//          analogWrite(pwmc, 0);
//          analogWrite(pwma, 20);
//          analogWrite(pwmb, LOW);
//          digitalWrite(pwmc, LOW);
//          digitalWrite(pwmd, LOW);
      maju();

}

void maju(){
          analogWrite(pwma, 0);
          analogWrite(pwmb, 0);
          analogWrite(pwmc, 130);
          analogWrite(pwmd, 0);
          
  
  }
void lampu_depan(){
    if(ld == true){
      digitalWrite(lamp_d, HIGH);
      }
}
      
void lampu_belakang(){
    if(lb == true){
      digitalWrite(lamp_b, HIGH);
      }
}
      
void riting_kanan(){
    if(rkn == true){
      digitalWrite(riting_kn, HIGH);
      delay(1000);
      digitalWrite(riting_kn, LOW);
      delay(1000);
      }
}

void riting_kiri(){
    if(rkr == true){
      digitalWrite(riting_kr, HIGH);
      delay(1000);
      digitalWrite(riting_kr, LOW);
      delay(1000);
      }
}
  

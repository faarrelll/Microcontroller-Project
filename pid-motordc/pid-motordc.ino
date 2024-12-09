#define M1EN 6
#define M1INA 2
#define M1INB 4
#define M1PWM 9
#define Enc_pin 3

//declaration and initialization variables
volatile long count = 0;
float deltaMillis, currentMillis, previousMillis;
int ts = 20;
float rpm = 0;
unsigned int analogPin = A0
double sense = 0;
double d2v = 0;
double currentSense = 0;
int PPR = 1000;

float Kp = 0.05;
float Ki = 0.0005;
float Kd = 0;

float in1 = 0;
float in2 = 0;

float set = 2000;
float error = 0;
float eint = 0; 
float eprev = 0;
float eprev_2 = 0;
float prevI = 0;
float u = 0;
float input = 0

//declaration of some function
void run_motor();
void daq();
void PID1();

void setup() 
{

  pinMode(M1EN,OUTPUT);
  pinMode(M1INA,OUTPUT);
  pinMode(M1INB,OUTPUT);
  pinMode(M1PWM,OUTPUT);
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(Enc_pin), counter, RISING);
  
  //0.128ms time period PWM setting
  TCCR1A=(1<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (1<<WGM10);
  TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
  TCNT1H=0x00;
  TCNT1L=0x00;
  ICR1H=0x00;
  ICR1L=0x00;
  OCR1AH=0x00;
  OCR1AL=0x00;
  OCR1BH=0x00;
  OCR1BL=0x00;
  
  //initialize motor speed at 0 voltage input
  u = 0;
  run_motor(1,1,u);
}

void loop() 
{ 
  //starting timer using millis
  currentMillis = millis();

  //obtain difference between current time and previous time
  deltaMillis = currentMillis - previousMillis; 
  
  //this prgram run if deltaMillis more than ts and stop if currentMillis less than 27.5s
  if (deltaMillis > ts && currentMillis < 27500)
  {
    //set previousMillis equal to currentMillis
    previousMillis = currentMillis;
    
    //rpm calculation
    rpm = (count*60*1000) / (PPR*deltaMillis);
    Serial.print(rpm);
    Serial.print("\t");
    input = (u/255)*24; // convert digital to voltage value
    Serial.println(u);
    count = 0;
    
    PID(Kp,Ki,Kd,set_rpm);
  }

  else if (deltaMillis >= 27500) run_motor(1,1,0);
}

void counter()
{
  count++;
}

void run_motor(boolean en, boolean dir, unsigned int pwm)
{
  if (en == 1)
  {
    digitalWrite(M1EN, HIGH);
    if (dir == 1)
    {
      digitalWrite(M1INA,HIGH);
      digitalWrite(M1INB,LOW);  
    }
    else
    {
      digitalWrite(M1INA,LOW);
      digitalWrite(M1INB,HIGH);
    }
    //analogWrite(M1PWM,pwm);
    OCR1A = pwm;
  }
  else
  {
    digitalWrite(M1EN, LOW);
    //analogWrite(M1PWM, 0);
    OCR1A = 0;
  }
}

void PID(float kp, float ki, float kd, float set)
{
  float error = set - rpm;
  float I = prevI + ki*((ts/2)*(error + 2*eprev + eprev));
  float u = kp*error + I + kd*(error - eprev);
  
  if (u>255) u = 255;
  else if (u<0) u = 0;
  else u = u;
  prevI = I;
  eprev = error; 
  eprev_2 = eprev;
  
  run_motor(1,1,u);
}

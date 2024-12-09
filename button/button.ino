#define button D3
#define button1 D7

int count = 0;
int count1 = 1;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(button,INPUT);
  pinMode(button1,INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  int nilaibutton = digitalRead(button);
//  Serial.println(nilaibutton);
  if(nilaibutton == 0){
    if(count == 0){
    Serial.println("kuy");
    count = 1;
    }
    } else{
      if(count == 1){
      Serial.println("hop");
      count = 0;
      }
      }
    int nilaibutton1 = digitalRead(button1);
    if(nilaibutton1 == 0){
    if(count1 == 0){
    Serial.println("kuy lah");
    count = 1;
    }
    } else{
      if(count1 == 1){
      Serial.println("hop lah");
      count = 0;
      }
      }
   
}

#define button 18
#define led 5

int led_state = LOW;
int button_state;
int last_button_state;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  pinMode(button,INPUT_PULLUP);
  button_state = digitalRead(button);

}

void loop() {
  // put your main code here, to run repeatedly:
  last_button_state = button_state;
  Serial.print("last button state = ");
  Serial.println(last_button_state);
  button_state = digitalRead(button);

  if(last_button_state == HIGH && button_state == LOW){
    Serial.println("The button is pressed");
    led_state = !led_state;
    digitalWrite(led, led_state);
  }
  Serial.print("button state = ");
  Serial.println(button_state);
  

}
